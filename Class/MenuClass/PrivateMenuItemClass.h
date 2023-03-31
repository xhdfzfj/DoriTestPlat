
#ifndef PRIVATEMENUITEMCLASS_H
#define PRIVATEMENUITEMCLASS_H

#include <QString>
#include <QRect>
#include <QColor>

class PrivateMenuItemClass
{
public:
    explicit PrivateMenuItemClass( QString pMenuName, QRect pRect );
    virtual ~PrivateMenuItemClass();

    QColor GetFrontColor() { return mFrontColor; }
    QRect GetItemRect() { return mItemRect; }
    QString GetMenuItemName() { return mMenuItemName; }
private:
    QRect mItemRect;
    QString mMenuItemName;
    QColor mFrontColor;
};

#endif // PRIVATEMENUITEMCLASS_H
