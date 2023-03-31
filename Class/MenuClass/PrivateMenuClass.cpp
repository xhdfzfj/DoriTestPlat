#include "PrivateMenuClass.h"
#include <QPainter>

PrivateMenuClass::PrivateMenuClass()
{
    mFont = QFont( "SimSun", mFontSize );
    mFontSize = 12;

    mDisplayPoint = QPoint( -1, -1 );
    mMenuRect = QRect( -1, -1, -1, -1 );
    mColor = Qt::black;
    mColor.setAlpha( 190 );
    mMenuImageP = nullptr;
    mSelectIndex = -1;
}


PrivateMenuClass::~PrivateMenuClass()
{
    PrivateMenuItemClass * _tmpItmP;

    if( mItemS.empty() )
    {
        foreach ( auto _itm, mItemS )
        {
            _tmpItmP = _itm;
            delete _tmpItmP;
        }

        mItemS.clear();
    }

    if( mMenuImageP != nullptr )
    {
        delete mMenuImageP;
        mMenuImageP = nullptr;
    }
}

/**
 * @brief PrivateMenuClass::sub_AddMenuItem
 * @param pMenuName
 */
void PrivateMenuClass::sub_AddMenuItem( QString pMenuName )
{
    QFontMetrics _tmpFm( mFont );
    PrivateMenuItemClass * _tmpMenuItemP;

    QRect _tmpRect;
    int _rectHeight;
    int _width;
    int _height;
    _tmpMenuItemP = nullptr;

    _width = _tmpFm.horizontalAdvance( pMenuName );
    _width += ( 5 + 5 );
    _height = _tmpFm.height();

    if( mMenuRect == QRect( -1, -1, -1, -1 ) )
    {
        mMenuRect = QRect( 0, 0, _width, _height + 5 + 5 );
        _tmpRect = QRect( 5, _height, _width, _height + 5 + 5 );
        _tmpMenuItemP = new PrivateMenuItemClass( pMenuName, _tmpRect );
    }
    else
    {
        if( _width > mMenuRect.width() )
        {
            mMenuRect.setWidth( _width );
        }

        _rectHeight = mMenuRect.height();

        _tmpRect = QRect( 5, _rectHeight + _height, mMenuRect.width(), _rectHeight + 5 + _height );

        _tmpMenuItemP = new PrivateMenuItemClass( pMenuName, _tmpRect );

        _rectHeight += ( 5 + _height );
        mMenuRect.setHeight( _rectHeight );
    }

    if( _tmpMenuItemP != nullptr )
    {
        mItemS.push_back( _tmpMenuItemP );
    }
}

/**
 * @brief PrivateMenuClass::sub_SetDisplayPoint
 * @param pPoint
 */
void PrivateMenuClass::sub_SetDisplayPoint( QPoint pPoint )
{
    if( pPoint != QPoint( -1, -1 ) )
    {
        if( mMenuImageP != nullptr )
        {
            delete mMenuImageP;
        }

        mMenuImageP = new QImage( mMenuRect.width(), mMenuRect.height(), QImage::Format_ARGB32 );
        mMenuImageP->fill( mColor );

        sub_DrawMenuItem();
    }
    mDisplayPoint = pPoint;
}

/**
 * @brief PrivateMenuClass::sub_DrawMenuItem
 */
void PrivateMenuClass::sub_DrawMenuItem()
{
    QRect _tmpRect;
    QColor _tmpColor;
    PrivateMenuItemClass * _tmpItemP;
    std::list< PrivateMenuItemClass * >::iterator _tmpItm;

    QPainter _tmpPainter( mMenuImageP );
    _tmpPainter.setRenderHint( QPainter::Antialiasing );
    _tmpPainter.setRenderHint( QPainter::TextAntialiasing );
    _tmpPainter.setFont( mFont );
    QPen _tmpPen;

    _tmpPainter.begin( mMenuImageP );

    for( _tmpItm = mItemS.begin(); _tmpItm != mItemS.end(); _tmpItm++ )
    {
        _tmpItemP = *_tmpItm;
        _tmpRect = _tmpItemP->GetItemRect();
        _tmpColor = _tmpItemP->GetFrontColor();
        _tmpPen.setColor( _tmpColor );
        _tmpPainter.setPen( _tmpPen );

        _tmpPainter.drawText( _tmpRect.x(), _tmpRect.y(), _tmpItemP->GetMenuItemName() );
    }
    _tmpPainter.end();
}

/**
 * @brief PrivateMenuClass::fun_GetDisplayMenuItemRect
 * @param pRetCount
 * @return
 */
QRect * PrivateMenuClass::fun_GetDisplayMenuItemRect( int & pRetCount )
{
    QRect * _retRectArrayP;

    _retRectArrayP = nullptr;

    pRetCount = 0;

    if( !mItemS.empty() )
    {
        pRetCount = mItemS.size();
        _retRectArrayP = new QRect[ pRetCount ];

        int i;
        PrivateMenuItemClass * _tmpItmP;
        std::list< PrivateMenuItemClass * >::iterator _Itm;

        i = 0;
        for( _Itm = mItemS.begin(); _Itm != mItemS.end(); _Itm++ )
        {
            _tmpItmP = *_Itm;
            _retRectArrayP[ i ] = _tmpItmP->GetItemRect();

            _retRectArrayP[ i ].setX( _retRectArrayP[ i ].x() + mDisplayPoint.x() );
            _retRectArrayP[ i ].setY( _retRectArrayP[ i ].y() + mDisplayPoint.y() );
            i += 1;
        }
    }

    return _retRectArrayP;
}

/**
 * @brief PrivateMenuClass::sub_SetSelectItem
 * @param pIndex
 */
void PrivateMenuClass::sub_SetSelectItem( int pIndex )
{

}
