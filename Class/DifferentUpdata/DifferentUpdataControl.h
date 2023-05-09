﻿
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
    Q_INVOKABLE void sub_WheelEvent( int pDirect, int pFlag ); //0代表向下  1代表向上
    Q_INVOKABLE void sub_CreateBsDiffFile( void );  //生成BSDIFF文件

public:
    DifferentUpdataControl( QQuickItem * pParent = nullptr );
    virtual ~DifferentUpdataControl();

    void sub_GetOtherFileDataAndLen( uint8_t * pFileDataP, int pFileDataLen );
    int GetDifferentDisplayType() { return mDifferentDisplayType; }

private:
    void sub_ReadyCreateGui( void );
    void sub_DataToImage( void );

    int mCurrDisplayStartY;
    int mCurrDisplayEndY;

    int mCurrStartDisplayLine;  //当前开始显示的行

private:
    MainModelClass * mMainModelObjP;
    int mDifferentDisplayType;      //显示的类型 0 旧  1 新  2结果
};

#endif // DIFFERENTUPDATACONTROL_H
