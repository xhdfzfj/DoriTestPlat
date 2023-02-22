#include "PrivateEventClass.h"
#include "SpiCaptureDataClass.h"

const spiCmdStruct_S mSpiCmdS[] = { { 0x05, "读取状态寄存器" },
                                    { 0x01, "写状态寄存器" },
                                    { 0x03, "读取数据" },
                                    { 0x02, "写数据" },
                                    { 0x20, "删除扇区" },
                                    { 0x60, "全片删除"},
                                    { 0x06, "写使能"},
                                    { 0xD8, "块删除"} };
/**
 * @brief SpiCaptureDataClass::SpiCaptureDataClass
 */
SpiCaptureDataClass::SpiCaptureDataClass( string pFilePath ) : FileAnalyseBaseClass( pFilePath )
{
//    mSpiDataBufP = new uint8_t [ 1024 ];

//    int _retValue;

//    _retValue = fun_GetFileData( mSpiDataBufP, 1024 );

//    mSpiDataBufLen = _retValue;
    mSpiDataBufP = nullptr;
    mSpiDataBufLen = 0;
    mActionFlag = false;
    mAnalyseBreakFlag = false;
}

/**
 * @brief SpiCaptureDataClass::~SpiCaptureDataClass
 */
SpiCaptureDataClass::~SpiCaptureDataClass()
{
    sub_ClearHandle();
}

/**
 * @brief SpiCaptureDataClass::sub_StartAnalyse
 */
void SpiCaptureDataClass::sub_StartAnalyse()
{
    PrivateEventClass * _tmpEventObjP;
    if( !mActionFlag )
    {
        mActionFlag = true;

        mAnalyseThreadP = new std::thread( &SpiCaptureDataClass::sub_SpiDataAnalyseHandle, this );

        _tmpEventObjP = new PrivateEventClass( EventType_e::logInfoType, DataType_e::StringType, "启动SPI数据分析线程" );    //不用清理,接口处清理
        _tmpEventObjP->SetLogLevel( xhdLogEventClass::logInfo );

        if( mParentEventInf )
        {
            mParentEventInf( ( void * )_tmpEventObjP );
        }
    }
    else
    {
        _tmpEventObjP = new PrivateEventClass( EventType_e::logInfoType, DataType_e::StringType, "SPI数据分析线程已启动" );    //不用清理,接口处清理
        _tmpEventObjP->SetLogLevel( xhdLogEventClass::logErr );

        if( mParentEventInf )
        {
            mParentEventInf( ( void * )_tmpEventObjP );
        }
    }
}

/**
 * @brief SpiCaptureDataClass::sub_SpiDataAnalyseHandle
 *      数据分析线程
 */
void SpiCaptureDataClass::sub_SpiDataAnalyseHandle()
{
    int _retValue;
    PrivateEventClass * _tmpEventObjP;

    if( mSpiDataBufP == nullptr )
    {
        mSpiDataBufP = new uint8_t [ 1024 ];
    }

    _retValue = fun_GetFileData( mSpiDataBufP, 1024 );
    while( _retValue > 0 )
    {
        if( mAnalyseBreakFlag )
        {
            break;
        }
        mSpiDataBufLen += _retValue;
        _retValue = fun_GetFileData( mSpiDataBufP, 1024 );
    }

    _tmpEventObjP = new PrivateEventClass( EventType_e::logInfoType, DataType_e::StringType, "SPI数据分析完成" );    //不用清理,接口处清理
    _tmpEventObjP->SetLogLevel( xhdLogEventClass::logInfo );

    if( mParentEventInf )
    {
        mParentEventInf( ( void * )_tmpEventObjP );
    }

}

/**
 * @brief SpiCaptureDataClass::sub_ClearHandle
 *      重载清理函数
 */
void SpiCaptureDataClass::sub_ClearHandle()
{
    if( mActionFlag )
    {
        mAnalyseBreakFlag = true;
        mAnalyseThreadP->join();
        delete mAnalyseThreadP;
        mAnalyseThreadP = nullptr;
    }
    if( mSpiDataBufP != nullptr )
    {
        delete [] mSpiDataBufP;
    }
}
