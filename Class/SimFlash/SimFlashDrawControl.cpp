#include "SimFlashDrawControl.h"

SimFlashDrawControl::SimFlashDrawControl(QQuickItem * pParent) : QQuickPaintedItem( pParent )
{
    mFont = QFont( "SimSun", 16 );
    mFontSize = 16;

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

    int _SignalDataWidth;
    int _SingleAscWidth;
    int _AddressStringWidth;
    int _colonStringWidth;
    QString _testString;

    QFontMetrics _tmpFm( mFont );

    _testString = "FF";
    _SignalDataWidth = _tmpFm.horizontalAdvance( _testString );
    _testString = "FFFFFFFF";
    _AddressStringWidth = _tmpFm.horizontalAdvance( _testString );
    _testString = ":";
    _colonStringWidth = _tmpFm.horizontalAdvance( _testString );
    _testString = "A";
    _SingleAscWidth = _tmpFm.horizontalAdvance( _testString );

    int _tmpWidth = _width - _AddressStringWidth - _colonStringWidth - 3 - 3;
    int _lineByteS = fun_CalcLineDisplayByteS( _tmpWidth, _SignalDataWidth, _SingleAscWidth  );

    if( _lineByteS != 0 )
    {

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
