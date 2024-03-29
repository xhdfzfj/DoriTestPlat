﻿#ifndef MAINMODELCLASS_H
#define MAINMODELCLASS_H

#include <QObject>
#include <QUrl>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "../Class/PrivateEventClass.h"
#include "../Class/SpiCaptureDataClass.h"
#include "./LogViewModel.h"
#include "../Control/GuiDrawControl.h"
#include "./spiviewmodel.h"
#include "./serialconfigmodel.h"
#include "./Class/SimFlash/FlashSimClass.h"
#include "./Class/DifferentUpdata/DifferentUpdataControl.h"


class MainModelClass : public QObject
{
    Q_OBJECT
public:
    explicit MainModelClass(QObject *parent = nullptr);
    virtual ~MainModelClass();

    Q_INVOKABLE void sub_OpenSerialPortClick( QString pButtonText );
    Q_INVOKABLE void sub_AnalyseSpiClick( QUrl pSpiFilePath );
    Q_INVOKABLE void sub_GetDrawObjectFromQml( QObject * pObjectP );
    Q_INVOKABLE void sub_TestSelf() { qDebug() << "Main Model test self"; }

    Q_INVOKABLE void sub_TestButClick();

    void sub_ChildObjectEventHandle( void * pEventP );
    void sub_EventHandle( void );

private:
    void sub_ClearEventQueue( void );
    void sub_DifferentUpdataObjSyncWheel( void * pSenderObjP, int pDirect );
    void sub_DifferentUpdataObjectGetOldFile( void * pSenderObjP, void * pCallBackP,int pType );
    //void sub_SimFlashContentGuiReady( FlashModifyContent_s * pFlashModifyContentP, FlashSimClass * pDestFlashP );

public: //将变量申明
    LogViewModel * mLogViewModelObjP;
    SpiViewModel * mSpiModelObjP;
    SerialConfigModel * mSerialConfigModelObjP;
    //SimFlashListModelClass * mSimFlashModifyModelObjP;

private:
    SpiCaptureDataClass * mSpiAnalyseObjP;
    GuiDrawControl * mSpiAnalyseGuiObjP;    //不用进行释放

    std::mutex mEventQLock;
    std::queue< PrivateEventClass * > mEventQ;

    std::thread * mEventHandleThreadP;
    bool mEventHandleThreadStopFlag;
    std::mutex mSynmtx;
    std::condition_variable mSynCv;

    DifferentUpdataControl ** mDifferentUpdataObjSP;

signals:

};

#endif // MAINMODELCLASS_H
