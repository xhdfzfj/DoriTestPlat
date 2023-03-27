#include "SimFlashDrawControl.h"

SimFlashDrawControl::SimFlashDrawControl(QQuickItem * pParent) : QQuickPaintedItem( pParent )
{
    mFont = QFont( "SimSun", 16 );
    mFontSize = 16;

    mCurrDisplayIndex = 0;
    mCurrDisplayEndIndex = 0;
    mLineByteS = 0;

    mAddressTableFontColor = Qt::white;
    mAddressTableBackColor = Qt::black;
    mBackColor = Qt::cyan;
    mMainImageP = nullptr;

    connect( this, SIGNAL( sub_SignalReDraw() ), this, SLOT( sub_SlotReDraw() ), Qt::QueuedConnection );

}


SimFlashDrawControl::~SimFlashDrawControl()
{

}

/**
 * @brief SimFlashDrawControl::sub_QmlLoadered
 *      Qml装载完成
 */
void SimFlashDrawControl::sub_QmlLoadered()
{
    qDebug() << "simflash width " << width();
    sub_DataToImage();
}

/**
 * @brief SimFlashDrawControl::fun_CalcLineDisplayByteS
 * @param pWidth
 * @param pDataDisplayWidth
 * @param pAscDisplayWidth
 * @return
 *      一行最多显示的字节数(4的倍数)
 */
int SimFlashDrawControl::fun_CalcLineDisplayByteS( int pWidth, int pDataDisplayWidth, int pAscDisplayWidth )
{
    int _retValue;

    _retValue = 0;

    int i, j, _tmpU32Value;

    i = 4;
    j = 0;
    _tmpU32Value = i * pDataDisplayWidth + ( i - 1 ) * pAscDisplayWidth + i * pAscDisplayWidth;

    while( _tmpU32Value <= pWidth )
    {
        j = i;
        i *= 2;
        _tmpU32Value = i * pDataDisplayWidth + ( i - 1 ) * pAscDisplayWidth + i * pAscDisplayWidth;
    }

    if( j != 0 )
    {
        _retValue = j;
    }

    return _retValue;
}

/**
 * @brief SimFlashDrawControl::sub_DataToImag
 *      把数据绘制到IMAGE中
 */
void SimFlashDrawControl::sub_DataToImage()
{
    int _width;
    int _height;

    _width = width();
    _height = height();

    if( mMainImageP == nullptr )
    {
        mMainImageP = new QImage( _width, _height, QImage::Format_ARGB32 );
        mMainImageP->fill( mBackColor );
    }
    else
    {
        if( ( mMainImageP->width() != _width ) || ( mMainImageP->height() != _height ) )
        {
            delete mMainImageP;
        }
        mMainImageP = new QImage( _width, _height, QImage::Format_ARGB32 );
        mMainImageP->fill( mBackColor );
    }

    QPainter _tmpPainter( mMainImageP );
    int _stringHeight;
    int _SignalDataWidth;
    int _SingleAscWidth;
    int _AddressStringWidth;
    int _colonStringWidth;
    QString _testString;

    QFontMetrics _tmpFm( mFont );

    _tmpPainter.setFont( mFont );

    _testString = "FF";
    _SignalDataWidth = _tmpFm.horizontalAdvance( _testString );
    _testString = "FFFFFFFF";
    _AddressStringWidth = _tmpFm.horizontalAdvance( _testString );
    _testString = ":";
    _colonStringWidth = _tmpFm.horizontalAdvance( _testString );
    _testString = "A";
    _SingleAscWidth = _tmpFm.horizontalAdvance( _testString );
    _stringHeight = _tmpFm.height();

    int _tmpWidth = _width - _AddressStringWidth - _colonStringWidth - 3 - 3 - _colonStringWidth;
    int _lineByteS = fun_CalcLineDisplayByteS( _tmpWidth, _SignalDataWidth, _SingleAscWidth  );

    if( _lineByteS != 0 )
    {
        int _x, _y;
        int i, j;
        char * _tmpDisplayBufP;
        char _tmpTransferBuf[ 256 ];

        mLineByteS = _lineByteS;

        _y = 3; //上边间隔

        _tmpDisplayBufP = new char [ _lineByteS ];

        j = 0;
        _y = 0;
        while( _y < _height )
        {
            _y += _stringHeight;
            i = fun_GetData( mCurrDisplayIndex + j, _lineByteS, _tmpDisplayBufP );
            if( i > 0 )
            {
                mCurrDisplayEndIndex += i;
                _x = 3; //左边间隔

                sprintf( _tmpTransferBuf, "%08X", ( mCurrDisplayIndex + j ) );
                _testString = QString::fromStdString( _tmpTransferBuf );
                _tmpPainter.drawText( _x, _y, _testString );
                _x += _SignalDataWidth * 4;

                _testString = ":";
                _tmpPainter.drawText( _x, _y, _testString );
                _x += _colonStringWidth;

                for( int z = 0; z < i; z++ )
                {
                    sprintf( _tmpTransferBuf, "%02X", ( uint8_t )( _tmpDisplayBufP[ z ] ) );
                    _testString = QString::fromStdString( _tmpTransferBuf );
                    _tmpPainter.drawText( _x, _y, _testString );
                    _x += _SignalDataWidth;

                    _testString = " ";
                    _tmpPainter.drawText( _x, _y, _testString );
                    _x += _SingleAscWidth;
                }

                _x += _colonStringWidth;

                for( int z = 0; z < i; z++ )
                {
                    _tmpTransferBuf[ 0 ] = _tmpDisplayBufP[ z ];
                    _tmpTransferBuf[ 1 ] = 0;
                    _testString = QString::fromStdString( _tmpTransferBuf );
                    _tmpPainter.drawText( _x, _y, _testString );
                    _x += _SingleAscWidth;

                }

            }
            else
            {
                break;
            }
            j += i;
        }

        delete [] _tmpDisplayBufP;
    }
}

/**
 * @brief SimFlashDrawControl::sub_SlotReDraw
 */
void SimFlashDrawControl::sub_SlotReDraw()
{

}


/**
 * @brief SimFlashDrawControl::paint
 * @param pPainter
 */
void SimFlashDrawControl::paint( QPainter * pPainter )
{
    if( mMainImageP != nullptr )
    {
        pPainter->setRenderHint( QPainter::Antialiasing );
        pPainter->drawImage( 0, 0, *mMainImageP );
    }
}
