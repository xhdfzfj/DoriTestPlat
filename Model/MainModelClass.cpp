#include <QDebug>
#include "../Class/SimFlash/SimFlashDrawControl.h"
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
    mSerialConfigModelObjP = new SerialConfigModel(this);
    //mSimFlashModifyModelObjP = new SimFlashListModelClass( this );

    mEventHandleThreadStopFlag = false;

    mEventHandleThreadP = new std::thread( &MainModelClass::sub_EventHandle, this );

    mDifferentUpdataObjSP = nullptr;
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
    delete mSerialConfigModelObjP;

    if( mDifferentUpdataObjSP != nullptr )
    {
        delete [] mDifferentUpdataObjSP;
    }
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
    mSpiAnalyseGuiObjP = ( GuiDrawControl * )pObjectP;
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

///**
// * @brief MainModelClass::sub_SimFlashContentGuiReady
// * @param pFlashModifyContentP
// * @param pDestFlashP
// */
//void MainModelClass::sub_SimFlashContentGuiReady( FlashModifyContent_s * pFlashModifyContentP, FlashSimClass * pDestFlashP )
//{
//    char * _tmpByteS;
//    uint32_t * _tmpAddressValue;
//    char _tmpTransferBuf[ 256 ];
//    QString mAddressStr;
//    QString mContentStr;
//    int _tmpAddress;
//    int i;
//    int mLineByteCount;
//    int _retValue;

//    mLineByteCount = pFlashModifyContentP->mGuiLineDisplayByteCount;
//    //mSimFlashModifyModelObjP->sub_ClearContent();

//    for( i = 0; i < 1/*pFlashModifyContentP->mLineCount*/; i++ )
//    {
//        mContentStr = "";
//        _tmpByteS = new char [ pFlashModifyContentP->mLineByteCount ];
//        _tmpAddress = pFlashModifyContentP->mStartAddress + i * mLineByteCount + pFlashModifyContentP->mStartOffset;
//        _retValue = pDestFlashP->fun_GetData( _tmpAddress, pFlashModifyContentP->mLineByteCount, _tmpByteS );

//        sprintf( _tmpTransferBuf, "%08X", _tmpAddress );
//        mAddressStr = QString::fromStdString( _tmpTransferBuf );
//        mAddressStr += ":";

//        for( int z = 0; z < pFlashModifyContentP->mLineByteCount; z++ )
//        {
//            if( z != 0 )
//            {
//                mContentStr += ",";
//            }
//            sprintf( _tmpTransferBuf, "%02X", _tmpByteS[ z ] );
//            mContentStr += QString::fromStdString( _tmpTransferBuf );
//        }

//        //mSimFlashModifyModelObjP->sub_AddContent( mAddressStr, mContentStr );
//    }
//}

/**
 * @brief MainModelClass::sub_DifferentUpdataObjSyncWheel
 *      同步差分生成窗口的同步滚动
 * @param pSenderObjP
 * @param pDirect
 */
void MainModelClass::sub_DifferentUpdataObjSyncWheel( void * pSenderObjP, int pDirect )
{
    if( mDifferentUpdataObjSP != nullptr )
    {
        for( int i = 0; i < 3; i++ )
        {
            if( ( ( void * )mDifferentUpdataObjSP[ i ] != pSenderObjP ) &&
                ( mDifferentUpdataObjSP[ i ] != nullptr ) )
            {
                if( mDifferentUpdataObjSP[ i ]->GetDifferentDisplayType() != 2 )
                {
                    mDifferentUpdataObjSP[ i ]->sub_WheelEvent( pDirect, 0 );
                }
            }
        }
    }
}

/**
 * @brief MainModelClass::sub_DifferentUpdataObjectGetOldFile
 * @param pSenderObjP
 * @param pCallBackP
 */
void MainModelClass::sub_DifferentUpdataObjectGetOldFile( void * pSenderObjP, void * pCallBackP, int pType )
{
    void( * _tmpCallBackP )( uint8_t *, int, void * );
    uint8_t * _tmpDataP;
    int _tmpLen;

    _tmpDataP = nullptr;
    _tmpLen = 0;
    _tmpCallBackP = ( void ( * )( uint8_t *, int, void * ) )( pCallBackP );

    if( _tmpCallBackP != nullptr )
    {
        if( mDifferentUpdataObjSP != nullptr )
        {
            if( pType == 0 )
            {
                if( mDifferentUpdataObjSP[ 0 ] != nullptr )
                {
                    _tmpLen = mDifferentUpdataObjSP[ 0 ]->GetFileLen();
                    _tmpDataP = mDifferentUpdataObjSP[ 0 ]->fun_GetAllFileData();
                }
            }
            else
            {
                if( mDifferentUpdataObjSP[ 1 ] != nullptr )
                {
                    _tmpLen = mDifferentUpdataObjSP[ 1 ]->GetFileLen();
                    _tmpDataP = mDifferentUpdataObjSP[ 1 ]->fun_GetAllFileData();
                }
            }
        }
        _tmpCallBackP( _tmpDataP, _tmpLen, pSenderObjP );
        if( _tmpDataP != nullptr )
        {
            delete _tmpDataP;
        }
    }
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
            mEventQLock.lock();
            _tmpEventObjP = mEventQ.front();
            mEventQ.pop();
            qDebug() << "sys event queue:" << mEventQ.size();
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
            else if( _tmpEventObjP->mEventType_e == EventType_e::HexDataDisplayType )
            {
                if( _tmpEventObjP->mSender_e == Sender_e::SpiCapture )
                {
                    mSpiAnalyseGuiObjP->sub_HexDataInput( ( uint8_t * )_tmpEventObjP->mVoidParam1P, _tmpEventObjP->mVoidParam1Len, _tmpEventObjP->mIntParam1 );
                }

                delete _tmpEventObjP;
            }
            else if( _tmpEventObjP->mEventType_e == EventType_e::ReDrawHexData )
            {
                if( _tmpEventObjP->mSender_e == Sender_e::SpiCapture )
                {
                    mSpiAnalyseGuiObjP->sub_ReDrawHexData();
                }

                delete _tmpEventObjP;
            }
            else if( _tmpEventObjP->mEventType_e == EventType_e::ModifySimFlashContent )
            {
                if( _tmpEventObjP->mSender_e == Sender_e::SimFlash )
                {
                    //sub_SimFlashContentGuiReady( ( FlashModifyContent_s * )_tmpEventObjP->mVoidParam1P, ( FlashSimClass * )_tmpEventObjP->mVoidParam2P );

                    delete ( FlashModifyContent_s * )_tmpEventObjP->mVoidParam1P;
                    delete _tmpEventObjP;
                }
            }
            else if( _tmpEventObjP->mEventType_e == EventType_e::RegisterObject )
            {
                if( _tmpEventObjP->mSender_e == Sender_e::DifferentUpdate )
                {
                    if( mDifferentUpdataObjSP == nullptr )
                    {
                        mDifferentUpdataObjSP = new DifferentUpdataControl * [ 3 ];
                        mDifferentUpdataObjSP[ 0 ] = nullptr;
                        mDifferentUpdataObjSP[ 1 ] = nullptr;
                        mDifferentUpdataObjSP[ 2 ] = nullptr;
                    }

                    if( mDifferentUpdataObjSP[ 0 ] == nullptr )
                    {
                        mDifferentUpdataObjSP[ 0 ] = ( DifferentUpdataControl * )_tmpEventObjP->mVoidParam1P;
                    }
                    else if( mDifferentUpdataObjSP[ 1 ] == nullptr )
                    {
                        mDifferentUpdataObjSP[ 1 ] = ( DifferentUpdataControl * )_tmpEventObjP->mVoidParam1P;
                    }
                    else
                    {
                        mDifferentUpdataObjSP[ 2 ] = ( DifferentUpdataControl * )_tmpEventObjP->mVoidParam1P;
                    }
                    delete _tmpEventObjP;
                }
            }
            else if( _tmpEventObjP->mEventType_e == EventType_e::WheelEvent )
            {
                if( _tmpEventObjP->mSender_e == Sender_e::DifferentUpdate )
                {
                    sub_DifferentUpdataObjSyncWheel( _tmpEventObjP->mVoidParam1P, _tmpEventObjP->mVoidParam1Len );
                    delete _tmpEventObjP;
                }
            }
            else if( _tmpEventObjP->mEventType_e == EventType_e::GetOldFileDataAndLen )
            {
                if( _tmpEventObjP->mSender_e == Sender_e::DifferentUpdate )
                {
                    sub_DifferentUpdataObjectGetOldFile( _tmpEventObjP->mVoidParam1P, _tmpEventObjP->mVoidParam2P, 0 );
                    delete _tmpEventObjP;
                }
            }
            else if( _tmpEventObjP->mEventType_e == EventType_e::GetNewFileDataAndLen )
            {
                if( _tmpEventObjP->mSender_e == Sender_e::DifferentUpdate )
                {
                    sub_DifferentUpdataObjectGetOldFile( _tmpEventObjP->mVoidParam1P, _tmpEventObjP->mVoidParam2P, 1 );
                    delete _tmpEventObjP;
                }
            }
            else if( _tmpEventObjP->mEventType_e == EventType_e::ClearBsDiffNewFileDisplay )
            {
                if( _tmpEventObjP->mSender_e == Sender_e::DifferentUpdate )
                {
                    if( mDifferentUpdataObjSP != nullptr )
                    {
                        if( mDifferentUpdataObjSP[ 1 ] != nullptr )
                        {
                            mDifferentUpdataObjSP[ 1 ]->sub_ClearCurrentDisplayData();
                        }
                    }
                    delete _tmpEventObjP;
                }
            }
            else if( _tmpEventObjP->mEventType_e == EventType_e::SetBsDiffNewDataSource )
            {
                if( _tmpEventObjP->mSender_e == Sender_e::DifferentUpdate )
                {
                    if( mDifferentUpdataObjSP != nullptr )
                    {
                        if( mDifferentUpdataObjSP[ 1 ] != nullptr )
                        {
                            mDifferentUpdataObjSP[ 1 ]->sub_SetDataSource( _tmpEventObjP->mVoidParam1P, _tmpEventObjP->mVoidParam1Len );
                            mDifferentUpdataObjSP[ 1 ]->sub_RequestReDrawdata();
                        }
                    }
                    delete _tmpEventObjP;
                }
            }
        }
    }
}

/**
 * @brief MainModelClass::sub_TestButClick
 */
void MainModelClass::sub_TestButClick()
{
    mSpiAnalyseGuiObjP->sub_TestFunction();
}

