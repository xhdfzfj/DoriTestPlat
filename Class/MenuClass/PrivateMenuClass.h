#ifndef PRIVATEMENUCLASS_H
#define PRIVATEMENUCLASS_H
#include <QDebug>
#include <QFont>
#include <QImage>
#include <list>
#include <QColor>
#include <QFontMetrics>
#include "./PrivateMenuItemClass.h"

class PrivateMenuClass
{
public:
    explicit PrivateMenuClass();
    virtual ~PrivateMenuClass();

    void sub_AddMenuItem( QString pMenuName );
    QPoint fun_GetMenuDisplayPoint() { return mDisplayPoint; }
    void sub_SetDisplayPoint( QPoint pPoint );
    QImage * GetMenuImage() { return mMenuImageP; }
    QRect * fun_GetDisplayMenuItemRect( int & pCount );
    void sub_SetSelectItem( int pIndex );

    int GetSelectIndex() { return mSelectIndex; }
    bool isDisplay()    { return mMenuDisplayFlag; }

protected:
    int GetItemCount() { return mItemS.size(); }

private:
    QFont mFont;
    int mFontSize;
    QPoint mDisplayPoint;
    QRect mMenuRect;
    QColor mColor;
    QImage * mMenuImageP;
    std::list< PrivateMenuItemClass * > mItemS;
    int mSelectIndex;
    bool mMenuDisplayFlag;

    void sub_DrawMenuItem();

};

#endif // PRIVATEMENUCLASS_H
