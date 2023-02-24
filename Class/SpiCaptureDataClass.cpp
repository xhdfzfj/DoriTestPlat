#include <QDebug>
#include "PrivateEventClass.h"
#include "SpiCaptureDataClass.h"
#include "../Include/CommonStruct.h"

const int mSpiCmdCount = 8;
const spiCmdStruct_S mSpiCmdS[] = { { 0x05, "读取状态寄存器", 2 },
                                    { 0x01, "写状态寄存器", 2 },
                                    { 0x03, "读取数据", -1 },
                                    { 0x02, "写数据", -1 },
                                    { 0x20, "删除扇区", 4 },
                                    { 0x60, "全片删除", 1 },
                                    { 0x06, "写使能", 1 },
                                    { 0xD8, "块删除", 4 } };
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
 * @brief SpiCaptureDataClass::fun_SpiCmdAnalyse
 *      处理捕捉到的SPI指令
 * @param pCmdDataP
 * @param pCmdDataLen
 * @param pDestSpiCmdStructP
 * @return
 *      -1 代表当前长度不够
 *      其它 代表越过的长度
 */
int SpiCaptureDataClass::fun_SpiCmdAnalyse( uint8_t * pCmdDataP, int pCmdDataLen, spiCmdStruct_S * pDestSpiCmdStructP )
{
    int _retValue;
    int _len;
    SpiCmdInfoClass * _spiCmdInfoP;
    PrivateEventClass * _tmpEventObjP;
    uint8_t * _tmpP;

    _retValue = 0;

    if( pDestSpiCmdStructP->mByteS != 0 )
    {
        _len = pDestSpiCmdStructP->mByteS * 2;
        if( pCmdDataLen < _len )
        {
            _retValue = -1; //长度不够
        }
        else
        {
            _tmpP = new uint8_t [ pDestSpiCmdStructP->mByteS ];

            int j;

            j = 0;
            for( int i = 0; i < _len; i++ )
            {
                if( ( i & 0x01 ) == 0 )
                {
                    _tmpP[ j ] = pCmdDataP[ i ];
                    j += 1;
                }
            }

            _spiCmdInfoP = new SpiCmdInfoClass( pDestSpiCmdStructP->mCmdDescribeStr, pDestSpiCmdStructP->mCmd, _tmpP, j );

            //_spiCmdInfoP 由系统事件处理程序释放
            _tmpEventObjP = new PrivateEventClass( EventType_e::SpiCmdInfoType, DataType_e::DataType, Sender_e::SpiCapture, ( void * )_spiCmdInfoP );
            _tmpEventObjP->SetFreeState( FreeParamType_e::SpiCmdInfoClassType );

            if( mParentEventInf )
            {
                mParentEventInf( ( void * )_tmpEventObjP );
            }

            _retValue = _len;
        }
    }

    return _retValue;
}

/**
 * @brief SpiCaptureDataClass::fun_FindSpiCmdInArray
 *      判断参数是否是SPI指令
 * @param pData
 * @return
 */
spiCmdStruct_S * SpiCaptureDataClass::fun_FindSpiCmdInArray( uint8_t pData )
{
    spiCmdStruct_S * _retP;

    _retP = nullptr;

    for( int i = 0; i < mSpiCmdCount; i++ )
    {
        if( pData == mSpiCmdS[ i ].mCmd )
        {
            _retP = ( spiCmdStruct_S * )( &mSpiCmdS[ i ] );
            break;
        }
    }
    return _retP;
}

/**
 * @brief SpiCaptureDataClass::sub_SpiDataAnalyseHandle
 *      数据分析线程
 */
void SpiCaptureDataClass::sub_SpiDataAnalyseHandle()
{
    int _retValue;
    int j;
    PrivateEventClass * _tmpEventObjP;
    spiCmdStruct_S * _spiCmdP;
    bool _flag;

    int _testCount = 0;

    if( mSpiDataBufP == nullptr )
    {
        mSpiDataBufP = new uint8_t [ 4096 ];
    }

    _flag = false;
    _spiCmdP = nullptr;
    _retValue = fun_GetFileData( &mSpiDataBufP[ mSpiDataBufLen ], 1024 );
    while( _retValue > 0 )
    {
        if( mAnalyseBreakFlag )
        {
            break;
        }
        mSpiDataBufLen += _retValue;

        for( int i = 0; i < mSpiDataBufLen; i++ )
        {
            _spiCmdP = fun_FindSpiCmdInArray( mSpiDataBufP[ i ] );
            if( _spiCmdP != nullptr )
            {
                //找到相应的SPI CMD
                _flag = true;
                j = fun_SpiCmdAnalyse( &mSpiDataBufP[ i ], mSpiDataBufLen - i, _spiCmdP );
                if( j == -1 )
                {
                    qDebug() << "长度不够";
                }
                else
                {
                    _testCount += 1;
                    i += j;
                    i -= 1;     //由于循环时会加1
                }

                if( _testCount > 1 )
                {
                    goto sub_SpiDataAnalyseHandle_exit;
                }
            }
            else
            {
                if( _flag )
                {
                    //出错了
                    qDebug() << "捕捉出错";
                }
            }
        }

        _retValue = fun_GetFileData( mSpiDataBufP, 1024 );
    }

    sub_SpiDataAnalyseHandle_exit:
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
