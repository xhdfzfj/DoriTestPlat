#include "GuiDrawControl.h"

/**
 * @brief GuiDrawControl::GuiDrawControl
 * @param pParent
 */
GuiDrawControl::GuiDrawControl(QQuickItem * pParent) : QQuickPaintedItem( pParent )
{
    mFont = QFont( "SimSun", 16 );
}

/**
 * @brief GuiDrawControl::~GuiDrawControl
 */
GuiDrawControl::~GuiDrawControl()
{

}

/**
 * @brief GuiDrawControl::sub_HexDataInput
 *      输入要显示的数据
 * @param pDataP
 * @param pDataLen
 * @param pStartOffset
 */
void GuiDrawControl::sub_HexDataInput( uint8_t * pDataP, int pDataLen, int pStartOffset )
{
    HexDataClass * _hexDataObjP;

    _hexDataObjP = new HexDataClass( pDataP, pDataLen, pStartOffset );
    if( mHexDataS.count( pStartOffset ) )
    {
        _hexDataObjP = mHexDataS.at( pStartOffset );
        delete _hexDataObjP;
    }

    mHexDataS[ pStartOffset ] = _hexDataObjP;

    sub_HexDataDraw();
}

/**
 * @brief GuiDrawControl::paint
 * @param pPainter
 */
void GuiDrawControl::paint( QPainter * pPainter )
{
    //paint( pPainter );  //调用基类的重绘函数
}
