
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
