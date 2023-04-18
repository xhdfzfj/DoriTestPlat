
#include "HexDataDisplayControl.h"

HexDataDisplayControl::HexDataDisplayControl( QQuickItem * pParent ) : QQuickPaintedItem( pParent )
{
    mMainImageP = nullptr;
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
