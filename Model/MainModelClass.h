#ifndef MAINMODELCLASS_H
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

class MainModelClass : public QObject
{
    Q_OBJECT
public:
    explicit MainModelClass(QObject *parent = nullptr);
    virtual ~MainModelClass();

    Q_INVOKABLE void sub_OpenSerialPortClick( QString pButtonText );
    Q_INVOKABLE void sub_AnalyseSpiClick( QUrl pSpiFilePath );
    Q_INVOKABLE void sub_ClearSelf();
    Q_INVOKABLE void sub_GetDrawObjectFromQml( QObject * pObjectP );

    void sub_ChildObjectEventHandle( void * pEventP );
    void sub_EventHandle( void );

private:
    void sub_ClearEventQueue( void );

public: //将变量申明
    LogViewModel * mLogViewModelObjP;
    SpiViewModel * mSpiModelObjP;

private:
    SpiCaptureDataClass * mSpiAnalyseObjP;
    GuiDrawControl * mSpiAnalyseGuiObjP;    //不用进行释放

    std::mutex mEventQLock;
    std::queue< PrivateEventClass * > mEventQ;

    std::thread * mEventHandleThreadP;
    bool mEventHandleThreadStopFlag;
    std::mutex mSynmtx;
    std::condition_variable mSynCv;

signals:

};

#endif // MAINMODELCLASS_H
