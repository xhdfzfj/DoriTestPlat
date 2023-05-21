#include "PrivateMenuClass.h"
#include <QPainter>

PrivateMenuClass::PrivateMenuClass()
{
    mFontSize = 12;
    mFont = QFont( "SimSun", mFontSize );

    mDisplayPoint = QPoint( -1, -1 );
    mMenuRect = QRect( -1, -1, -1, -1 );
    mColor = Qt::black;
    mColor.setAlpha( 190 );
    mMenuImageP = nullptr;
    mSelectIndex = -1;
    mMenuDisplayFlag = false;
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
        _tmpRect = QRect( 5, 5, _width - 5 - 5, _height );
        _tmpMenuItemP = new PrivateMenuItemClass( pMenuName, _tmpRect );
    }
    else
    {
        if( _width > mMenuRect.width() )
        {
            mMenuRect.setWidth( _width );
        }

        _rectHeight = mMenuRect.height();

        _tmpRect = QRect( 5, _rectHeight, mMenuRect.width() - 5 - 5, _height );

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

        mMenuDisplayFlag = true;
    }
    else
    {
        if( mMenuImageP != nullptr )
        {
            delete mMenuImageP;
            mMenuImageP = nullptr;
        }

        mMenuDisplayFlag = false;
    }
    mDisplayPoint = pPoint;
    mSelectIndex = -1;
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

        //_tmpPainter.fillRect( _tmpRect, QBrush( Qt::blue ) );
        _tmpPainter.drawText( _tmpRect.x(), _tmpRect.y() + _tmpRect.height() - 5, _tmpItemP->GetMenuItemName() );

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

            int _width = _retRectArrayP[ i ].width();
            int _height = _retRectArrayP[ i ].height();

            _retRectArrayP[ i ].setX( _retRectArrayP[ i ].x() + mDisplayPoint.x() );
            _retRectArrayP[ i ].setY( _retRectArrayP[ i ].y() + mDisplayPoint.y() );
            _retRectArrayP[ i ].setWidth( _width );
            _retRectArrayP[ i ].setHeight( _height );
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
    if( pIndex != -1 )
    {
        mSelectIndex = pIndex;

        PrivateMenuItemClass * _itemP;
        int i = 0;
        std::list< PrivateMenuItemClass * >::iterator _itm;
        for( _itm = mItemS.begin(); _itm != mItemS.end(); _itm++ )
        {
            if( i == mSelectIndex )
            {
                _itemP = *_itm;

                mMenuImageP->fill( mColor );

                sub_DrawMenuItem();

                QBrush _tmpBrush( Qt::blue );
                QPen _tmpPen( Qt::blue, 2 );
                QRect _tmpRect;
                QColor _tmpColor;
                QPainter _tmpPainter( mMenuImageP );

                _tmpRect = _itemP->GetItemRect();

                _tmpPainter.begin( mMenuImageP );

                _tmpPainter.setPen( _tmpPen );
                _tmpPainter.fillRect( _tmpRect, _tmpBrush );

                _tmpColor = _itemP->GetFrontColor();
                _tmpPen.setColor( _tmpColor );
                _tmpPainter.setPen( _tmpPen );

                _tmpPainter.drawText( _tmpRect.x(), _tmpRect.y() + _tmpRect.height() - 5, _itemP->GetMenuItemName() );
                _tmpPainter.end();

                break;
            }
            i += 1;
        }
    }
}
