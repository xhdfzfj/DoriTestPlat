#include <QDebug>
#include <iostream>
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
int SpiCaptureDataClass::fun_SpiCmdAnalyse( uint8_t * pCmdDataP, int pCmdDataLen, spiCmdStruct_S * pDestSpiCmdStructP, int pStartOffsetInFile )
{
    int _retValue;
    int _len;
    uint32_t _tmpAddr;
    uint32_t _tmpLen;
    spiCmdStruct_S * _tmpSpiCmd_S_P;
    SpiCmdInfoClass * _spiCmdInfoP;
    PrivateEventClass * _tmpEventObjP;
    uint8_t * _tmpP;
    std::string _tmpStr;
    int j;

    _retValue = 0;

    if( pDestSpiCmdStructP->mByteS != -1 )
    {
        _len = pDestSpiCmdStructP->mByteS * 2;
        if( pCmdDataLen < _len )
        {
            _retValue = -1; //长度不够
        }
        else
        {
            _tmpP = new uint8_t [ pDestSpiCmdStructP->mByteS ];

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
            _spiCmdInfoP->SetFileInfo( pStartOffsetInFile, j );

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
    else
    {
        //无法确定长度
        char _tmpDispBuf[ 32 ];
        int _operateLen;

        j = 2;
        while( j < pCmdDataLen )
        {
            _tmpSpiCmd_S_P = fun_FindSpiCmdInArray( pCmdDataP[ j ] );
            if( _tmpSpiCmd_S_P != nullptr )
            {
                j -= 2;
                _tmpLen = j / 2;
                _tmpLen += 1;   //要加上1
                _operateLen = _tmpLen;

                if( ( pDestSpiCmdStructP->mCmd == 0x03 ) || ( pDestSpiCmdStructP->mCmd == 0x02 ) )
                {
                    if( _tmpLen < 4 )
                    {
                        //长度不够
                        j += 2;     //恢复原来的j值
                        goto LOOP_GOTO_LABEL;
                    }
                    //读
                    _tmpAddr = pCmdDataP[ 2 ];
                    _tmpAddr <<= 8;
                    _tmpAddr |= pCmdDataP[ 4 ];
                    _tmpAddr <<= 8;
                    _tmpAddr |= pCmdDataP[ 6 ];
                    _tmpLen -= 4;
                }
                _tmpStr = pDestSpiCmdStructP->mCmdDescribeStr;
                sprintf( _tmpDispBuf, "0x%08x", _tmpAddr );
                _tmpStr += std::string( _tmpDispBuf );

                if( pDestSpiCmdStructP->mCmd == 0x03 )
                {
                    _tmpStr += ",读取长度:";
                    sprintf( _tmpDispBuf, "%d", _tmpLen );
                    _tmpStr += std::string( _tmpDispBuf );
                }

                if( pDestSpiCmdStructP->mCmd == 0x02 )
                {
                    _tmpStr += ",写数据长度:";
                    sprintf( _tmpDispBuf, "%d", _tmpLen );
                    _tmpStr += std::string( _tmpDispBuf );
                }

                if( _tmpSpiCmd_S_P->mCmd != 0x05 )
                {
                    j += 2;     //恢复原来的j值
                    goto LOOP_GOTO_LABEL;
                }
                else
                {
                    _spiCmdInfoP = new SpiCmdInfoClass( _tmpStr, pDestSpiCmdStructP->mCmd, nullptr, 0 );
                    _spiCmdInfoP->SetFileInfo( pStartOffsetInFile, _operateLen );

                    //_spiCmdInfoP 由系统事件处理程序释放
                    _tmpEventObjP = new PrivateEventClass( EventType_e::SpiCmdInfoType, DataType_e::DataType, Sender_e::SpiCapture, ( void * )_spiCmdInfoP );
                    _tmpEventObjP->SetFreeState( FreeParamType_e::SpiCmdInfoClassType );

                    if( mParentEventInf )
                    {
                        mParentEventInf( ( void * )_tmpEventObjP );
                    }

                    if( ( pDestSpiCmdStructP->mCmd == 0x03 ) || ( pDestSpiCmdStructP->mCmd == 0x02 ) )
                    {
                        //要向显示控件发送事件
                        uint8_t * _tmpP;

                        _tmpP = new uint8_t [ _tmpLen ];        //不用进行释放,释放EVENT时会释入该内存
                        memcpy( _tmpP, &pCmdDataP[ 4 ], _tmpLen );

                        _tmpEventObjP = new PrivateEventClass( EventType_e::HexDataDisplayType, DataType_e::DataType, Sender_e::SpiCapture,
                                                               ( void * )_tmpP, _tmpLen, pStartOffsetInFile );
                        if( mParentEventInf )
                        {
                            mParentEventInf( ( void * )_tmpEventObjP );
                        }
                    }
                }

                _retValue = j + 2;
                break;
            }
            LOOP_GOTO_LABEL:
            j += 2;
        }

        if( j >= pCmdDataLen )
        {
            _retValue = -1;
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
    int _FileOffset;
    bool _flag1;
    std::string _tmpStr;

    int _testCount = 0;

    if( mSpiDataBufP == nullptr )
    {
        mSpiDataBufP = new uint8_t [ 4096 ];
    }

    _FileOffset = 0;
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

        int i;
        _flag1 = false;
        for( i = 0; i < mSpiDataBufLen; i++ )
        {
            _spiCmdP = fun_FindSpiCmdInArray( mSpiDataBufP[ i ] );
            if( _spiCmdP != nullptr )
            {
                //找到相应的SPI CMD
                _flag = true;

                j = fun_SpiCmdAnalyse( &mSpiDataBufP[ i ], mSpiDataBufLen - i, _spiCmdP, _FileOffset + i );
                if( j == -1 )
                {
                    qDebug() << "长度不够";
                    memmove( mSpiDataBufP, &mSpiDataBufP[ i ], mSpiDataBufLen - i );
                    mSpiDataBufLen -= i;
                    _FileOffset += i;
                    _flag1 = true;
                    break;
                }
                else
                {
                    i += j;
                    i -= 1;     //由于循环时会加1

                    _testCount += 1;
                    if( _testCount > 15 )
                    {
                        DebugStopFlag = true;
                    }
                }
            }
            else
            {
                if( _flag )
                {
                    //出错了
                    qDebug() << "捕捉出错";
                    SpiCmdInfoClass * _spiCmdInfoP;

                    _spiCmdInfoP = new SpiCmdInfoClass( "捕捉出错", -1, nullptr, 0 );

                    _tmpEventObjP = new PrivateEventClass( EventType_e::SpiCmdInfoType, DataType_e::DataType, Sender_e::SpiCapture, ( void * )_spiCmdInfoP );
                    _tmpEventObjP->SetFreeState( FreeParamType_e::SpiCmdInfoClassType );
                    if( mParentEventInf )
                    {
                        mParentEventInf( ( void * )_tmpEventObjP );
                    }

                    _tmpStr = "捕捉出错";
                    if( _spiCmdInfoP->mStartOffsetInFile != -1 )
                    {
                        _tmpStr += ",文件偏移:" + ( QString::number( _spiCmdInfoP->mStartOffsetInFile, 16 ) ).toStdString();
                    }
                    _tmpEventObjP = new PrivateEventClass( EventType_e::logInfoType, DataType_e::StringType, _tmpStr );
                    _tmpEventObjP->SetLogLevel( xhdLogEventClass::LogLevel::logErr );
                    if( mParentEventInf )
                    {
                        mParentEventInf( ( void * )_tmpEventObjP );
                    }
                    _flag = false;
                }
            }
        }

        _retValue = fun_GetFileData( &mSpiDataBufP[ mSpiDataBufLen ], 1024 );
        if( !_flag1 )
        {
            _FileOffset += _retValue;
        }

        if( DebugStopFlag )
        {
            break;
        }
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
