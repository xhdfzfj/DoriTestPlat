
#ifndef DIFFERENTUPDATACONTROL_H
#define DIFFERENTUPDATACONTROL_H

#include <QUrl>
#include "../MenuClass/PrivateMenuClass.h"
#include "../../Control/HexDataDisplayControl.h"
#include "../../Class/FileAnalyseBaseClass.h"

class MainModelClass;

class DifferentUpdataControl : public HexDataDisplayControl, public FileAnalyseBaseClass
{
    Q_OBJECT

public:
    Q_INVOKABLE void sub_SetMainModelObj( QObject * pObjectP );
    Q_INVOKABLE void sub_DifferentType( int pState );
    Q_INVOKABLE void sub_DifferentFile( QUrl pSpiFilePath );
    Q_INVOKABLE void sub_WheelEvent( int pDirect, int pFlag ); //0代表向下  1代表向上
    Q_INVOKABLE void sub_CreateBsDiffFile( void );  //生成BSDIFF文件
    Q_INVOKABLE void sub_MouseRightButtonClick( qreal pX, qreal pY );

public:
    DifferentUpdataControl( QQuickItem * pParent = nullptr );
    virtual ~DifferentUpdataControl();

    static void sub_GetOldFileDataAndLenCB( uint8_t * pFileDataP, int pFileDataLen, void * pDestObjP );
    static void sub_GetNewFileDataAndLenCB( uint8_t * pFileDataP, int pFileDataLen, void * pDestObjP );

    int GetDifferentDisplayType() { return mDifferentDisplayType; }

private:
    void sub_ReadyCreateGui( void );
    void sub_DataToImage( void );
    void sub_GetOtherFileDataAndLen( uint8_t * pDataP, int pLen, int pType );   //0旧  1新

    int mCurrDisplayStartY;
    int mCurrDisplayEndY;

    int mCurrStartDisplayLine;  //当前开始显示的行

private:
    MainModelClass * mMainModelObjP;
    int mDifferentDisplayType;      //显示的类型 0 旧  1 新  2结果

    uint8_t * mOldDataContentP;
    int mOldDataContentLen;
    uint8_t * mNewDataContentP;
    int mNewDataContentLen;

    PrivateMenuClass * mContentMenuP;
    QRect * mContentItemRectP;
    int mContentItemRectCount;
};

#endif // DIFFERENTUPDATACONTROL_H
