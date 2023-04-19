
#include "HexDataDisplayControl.h"

HexDataDisplayControl::HexDataDisplayControl( QQuickItem * pParent ) : QQuickPaintedItem( pParent )
{
    mMainImageP = nullptr;

    mFont = QFont( "SimSun", 16 );
    mFontSize = 16;
    mFontColor = Qt::black;

    QFontMetrics _tmpFm( mFont );

    int _stringHeight;
    int _SignalDataWidth;
    int _SingleAscWidth;
    int _AddressStringWidth;
    int _colonStringWidth;
    QString _testString;

    _testString = "FF";
    _SignalDataWidth = _tmpFm.horizontalAdvance( _testString );
    _testString = "FFFFFFFF";
    _AddressStringWidth = _tmpFm.horizontalAdvance( _testString );
    _testString = ":";
    _colonStringWidth = _tmpFm.horizontalAdvance( _testString );
    _testString = "A";
    _SingleAscWidth = _tmpFm.horizontalAdvance( _testString );
    _stringHeight = _tmpFm.height();

    mFontHeight = _stringHeight;
    mDataFontWidth = _SignalDataWidth;
    mColonFontWidth = _colonStringWidth;
    mSingleAscWidth = _SingleAscWidth;
    mAddressStringWidth = _AddressStringWidth;

    mLineByteCount = 0;
    mCurrX = mSpaceValue;
    mCurrY = mSpaceValue;

    connect( this, SIGNAL( sub_SignalReDraw() ), this, SLOT( sub_SlotReDraw() ), Qt::QueuedConnection );
}

HexDataDisplayControl::~HexDataDisplayControl()
{
    if( mMainImageP != nullptr )
    {
        delete mMainImageP;
    }
}

/**
 * @brief HexDataDisplayControl::sub_SizeChange
 */
void HexDataDisplayControl::sub_SizeChange()
{
    qDebug() << "HexDataDisplayControl size change";

    if( ( width() != 0 ) && ( height() != 0 ) )
    {
        if( mMainImageP != nullptr )
        {
            delete mMainImageP;
            mMainImageP = nullptr;
        }

        mMainImageP = new QImage( width(), height(), QImage::Format_ARGB32 );
        mBackColor = fillColor();
        mMainImageP->fill( mBackColor );
    }
}

/**
 * @brief HexDataDisplayControl::fun_LineHexDataToImage
 * @param pDataBufP
 * @param pLen
 * @param pStartOffset
 * @return
 *      false 没有满一页
 *      true  已满一页
 */
bool HexDataDisplayControl::fun_LineHexDataToImage( uint8_t * pDataBufP, int pLen, int pStartOffset )
{
    bool _retFlag;
    bool _flag;
    QString _tmpStr;
    char _tmpTransferBuf[ 64 ];

    _retFlag = true;

    if( mMainImageP != nullptr )
    {
        QPainter _tmpPainter;

        _flag = _tmpPainter.begin( mMainImageP );

        _tmpPainter.setFont( mFont );
        mCurrY += mFontHeight;

        sprintf( _tmpTransferBuf, "%08X", pStartOffset );
        _tmpStr = QString::fromStdString( _tmpTransferBuf );
        _tmpPainter.drawText( mCurrX, mCurrY, _tmpStr );
        mCurrX += mAddressStringWidth;
        _tmpStr = ":";
        _tmpPainter.drawText( mCurrX, mCurrY, _tmpStr );
        mCurrX += mColonFontWidth;

        for( int z = 0; z < pLen; z++ )
        {
            if( z != 0 )
            {
                _tmpStr = " ";
                _tmpPainter.drawText( mCurrX, mCurrY, _tmpStr );
                mCurrX += mSingleAscWidth;
            }
            sprintf( _tmpTransferBuf, "%02X", ( uint8_t )( pDataBufP[ z ] ) );
            _tmpStr = QString::fromStdString( _tmpTransferBuf );
            _tmpPainter.drawText( mCurrX, mCurrY, _tmpStr );
            mCurrX += mDataFontWidth;
        }

        _tmpPainter.end();

        if( ( mCurrY + mFontHeight ) > mMainImageP->height() )
        {
            _retFlag = true;
        }
        else
        {
            _retFlag = false;
        }
    }

    return _retFlag;
}


/**
 * @brief SimFlashDrawControl::sub_SlotReDraw
 */
void HexDataDisplayControl::sub_SlotReDraw()
{
    update();
}


/**
 * @brief GuiDrawControl::paint
 * @param pPainter
 */
void HexDataDisplayControl::paint( QPainter * pPainter )
{
    //paint( pPainter );  //调用基类的重绘函数
    if( mMainImageP != nullptr )
    {
        pPainter->setRenderHint( QPainter::Antialiasing );
        pPainter->drawImage( 0, 0, *mMainImageP );
    }
}
