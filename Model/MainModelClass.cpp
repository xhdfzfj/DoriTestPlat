#include <QDebug>
#include "MainModelClass.h"

/**
 * @brief MainModelClass::MainModelClass
 * @param parent
 */
MainModelClass::MainModelClass(QObject *parent)
    : QObject{parent}
{
    mLogViewModelObjP = new LogViewModel( this );
    mSpiAnalyseObjP = nullptr;
    mSpiModelObjP = new SpiViewModel( this );

    mEventHandleThreadStopFlag = false;

    mEventHandleThreadP = new std::thread( &MainModelClass::sub_EventHandle, this );

}

/**
 * @brief MainModelClass::~MainModelClass
 */
MainModelClass::~MainModelClass()
{
    mEventHandleThreadStopFlag = true;
    mSynCv.notify_one();
    mEventHandleThreadP->join();
    delete mEventHandleThreadP;

    sub_ClearEventQueue();

    if( mSpiAnalyseObjP != nullptr )
    {
        delete mSpiAnalyseObjP;
    }
    if( mSpiModelObjP != nullptr )
    {
        delete mSpiModelObjP;
    }
    delete mLogViewModelObjP;
}

/**
 * @brief MainModelClass::sub_ClearSelf
 *      自清理
 */
void MainModelClass::sub_ClearSelf()
{
   delete this;
}

/**
 * @brief MainModelClass::sub_OpenSerialPortClick
 *      打开串按键
 */
void MainModelClass::sub_OpenSerialPortClick( QString pButtonText )
{
    mLogViewModelObjP->sub_EnterLog( xhdLogEventClass::LogLevel::logInfo, pButtonText );
}

/**
 * @brief MainModelClass::sub_GetDrawObjectFromQml
 * @param pObjectP
 */
void MainModelClass::sub_GetDrawObjectFromQml( QObject * pObjectP )
{
    GuiDrawControl * _tmpDrawObjP;

    _tmpDrawObjP = ( GuiDrawControl * )pObjectP;
}

/**
 * @brief MainModelClass::sub_AnalyseSpiClick
 *      开始分析SPI捕捉的文件
 * @param pSpiFilePath
 */
void MainModelClass::sub_AnalyseSpiClick( QUrl pSpiFilePath )
{
    std::function< void ( void * ) > _tmpFunc = nullptr;

    if( pSpiFilePath.isLocalFile() )
    {
        std::string _tmpStr;

        _tmpStr = pSpiFilePath.path().toStdString();

        mLogViewModelObjP->sub_EnterLog( xhdLogEventClass::LogLevel::logInfo, "SPI文件:" + QString::fromStdString( _tmpStr ) );

        if( mSpiAnalyseObjP != nullptr )
        {
            delete mSpiAnalyseObjP;
            mSpiAnalyseObjP = nullptr;
        }

        _tmpFunc = std::bind( &MainModelClass::sub_ChildObjectEventHandle, this, std::placeholders::_1 );
        mSpiAnalyseObjP = new SpiCaptureDataClass( _tmpStr );
        mSpiAnalyseObjP->sub_SetParentEventInterface( _tmpFunc );

        mSpiAnalyseObjP->sub_StartAnalyse();
        //mSpiAnalyseObjP->sub_Test();
    }
    else
    {
        mLogViewModelObjP->sub_EnterLog( xhdLogEventClass::LogLevel::logErr, "请选择SPI文件" );
    }
}

/**
 * @brief MainModelClass::sub_ChildObjectEventHandle
 *      各个子对象的事件处理接口
 * @param pEventP
 */
void MainModelClass::sub_ChildObjectEventHandle( void * pEventP )
{
    PrivateEventClass * _tmpEventObjP;

    _tmpEventObjP = ( PrivateEventClass * )pEventP;

    mEventQLock.lock();
    mEventQ.push( _tmpEventObjP );

    qDebug() << "mEentQ count:" << mEventQ.size();
    mEventQLock.unlock();
    mSynCv.notify_one();
}

/**
 * @brief MainModelClass::sub_ClearEventQueue
 *      清理事件队列
 */
void MainModelClass::sub_ClearEventQueue( void )
{
    PrivateEventClass * _eventObjP;

    mEventQLock.lock();

    while( !mEventQ.empty() )
    {
        _eventObjP = mEventQ.front();
        mEventQ.pop();

        delete _eventObjP;
    }

    mEventQLock.unlock();
}

/**
 * @brief MainModelClass::sub_EventHandle
 *      事件的线程处理
 */
void MainModelClass::sub_EventHandle( void )
{
    PrivateEventClass * _tmpEventObjP;

    while( !mEventHandleThreadStopFlag )
    {
        std::unique_lock< std::mutex > mSynLock( mSynmtx );
        mSynCv.wait( mSynLock );
        if( mEventHandleThreadStopFlag )
        {
            break;
        }
        while( !mEventQ.empty() )
        {
            _tmpEventObjP = mEventQ.front();
            mEventQLock.lock();
            mEventQ.pop();
            mEventQLock.unlock();

            if( _tmpEventObjP->mEventType_e == EventType_e::logInfoType )
            {
                mLogViewModelObjP->sub_EnterLog( _tmpEventObjP->mLoglevel, QString::fromStdString( _tmpEventObjP->mInfoStr ) );
                delete _tmpEventObjP;
            }
            else if( _tmpEventObjP->mEventType_e == EventType_e::SpiCmdInfoType )
            {
                mSpiModelObjP->sub_EnterSpiCmdInfo( ( SpiCmdInfoClass * )_tmpEventObjP->mVoidParam1P );
                delete _tmpEventObjP;
            }
        }
    }
}

