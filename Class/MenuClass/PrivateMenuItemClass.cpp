
#include "PrivateMenuItemClass.h"

PrivateMenuItemClass::PrivateMenuItemClass( QString pMenuName, QRect pRect )
{
    mFrontColor = Qt::white;
    mMenuItemName = pMenuName;
    mItemRect = pRect;
}


PrivateMenuItemClass::~PrivateMenuItemClass()
{

}

