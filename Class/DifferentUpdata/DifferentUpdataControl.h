
#ifndef DIFFERENTUPDATACONTROL_H
#define DIFFERENTUPDATACONTROL_H

#include <QUrl>
#include "../../Control/HexDataDisplayControl.h"
#include "../../Class/FileAnalyseBaseClass.h"

class MainModelClass;

class DifferentUpdataControl : public HexDataDisplayControl, public FileAnalyseBaseClass
{
    Q_OBJECT

public:
    Q_INVOKABLE void sub_SetMainModelObj( QObject * pObjectP );
    Q_INVOKABLE void sub_DifferentType( int pState )    { mDifferentDisplayType = pState; }
    Q_INVOKABLE void sub_DifferentFile( QUrl pSpiFilePath );

public:
    DifferentUpdataControl( QQuickItem * pParent = nullptr );
    virtual ~DifferentUpdataControl();

    int GetDifferentDisplayType() { return mDifferentDisplayType; }

private:
    void sub_ReadyCreateGui( void );

private:
    MainModelClass * mMainModelObjP;
    int mDifferentDisplayType;      //显示的类型 0 旧  1 新  2结果
};

#endif // DIFFERENTUPDATACONTROL_H
