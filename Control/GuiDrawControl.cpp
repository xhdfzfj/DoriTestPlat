﻿#include "GuiDrawControl.h"

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
    std::map< int, HexDataClass * >::iterator _itm;
    HexDataClass * _hexDataObjP;

    if( !mHexDataS.empty() )
    {
        for( _itm = mHexDataS.begin(); _itm != mHexDataS.end(); _itm++ )
        {
            _hexDataObjP = _itm->second;
            delete _hexDataObjP;
        }

        mHexDataS.clear();
    }
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
 * @brief GuiDrawControl::fun_CalcLineDisplayCount
 *      计算当前宽度下一行能显示的个数(宽度已减去前导部份)
 * @param pWidth
 * @param pHexDataWidth
 * @param pSpaceWidth
 * @return
 *      -1 代表当前宽度过小无法进行显示
 */
int GuiDrawControl::fun_CalcLineDisplayCount( int pWidth, int pHexDataWidth, int pSpaceWidth )
{

}

/**
 * @brief GuiDrawControl::sub_HexDataDraw
 */
void GuiDrawControl::sub_HexDataDraw( void )
{
    int _width, _height;
    int _tmpWidth;
    int _stringWidth;
    int _spaceWidth;
    int _colonWidth;

    HexDataClass * _hDataObjP;
    QFontMetrics _tmpFm( mFont );

    QString _tmpTestString = "FF";

    _stringWidth = _tmpFm.horizontalAdvance( _tmpTestString );

    _tmpTestString = "F";
    _spaceWidth = _tmpFm.horizontalAdvance( _tmpTestString );

    _tmpTestString = ":";
    _colonWidth = _tmpFm.horizontalAdvance( _tmpTestString );

    _width = width();
    _height = height();

    int _lineBytes = fun_CalcLineDisplayCount( _tmpWidth, _stringWidth, _spaceWidth );
    if( _lineBytes < 0 )
    {
        return;
    }

    _tmpWidth = _width - 2 * _colonWidth - 4 * _stringWidth;
    if( !mHexDataS.empty() )
    {
        for( auto _tmpItm : mHexDataS )
        {
            _hDataObjP = _tmpItm.second;


        }
    }
}

/**
 * @brief GuiDrawControl::paint
 * @param pPainter
 */
void GuiDrawControl::paint( QPainter * pPainter )
{
    //paint( pPainter );  //调用基类的重绘函数
}
