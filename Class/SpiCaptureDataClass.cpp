#include "SpiCaptureDataClass.h"

const spiCmdStruct_S mSpiCmdS[] = { { 0x05, "读取状态寄存器" },
                                    { 0x01, "写状态寄存器" },
                                    { 0x03, "读取数据" },
                                    { 0x02, "写数据" },
                                    { 0x20, "删除扇区" },
                                    { 0x60, "全片删除"}, };
/**
 * @brief SpiCaptureDataClass::SpiCaptureDataClass
 */
SpiCaptureDataClass::SpiCaptureDataClass( string pFilePath ) : FileAnalyseBaseClass( pFilePath )
{
    mSpiDataBufP = new uint8_t [ 1024 ];

    int _retValue;

    _retValue = fun_GetFileData( mSpiDataBufP, 1024 );

    mSpiDataBufLen = _retValue;
}

/**
 * @brief SpiCaptureDataClass::~SpiCaptureDataClass
 */
SpiCaptureDataClass::~SpiCaptureDataClass()
{
    delete [] mSpiDataBufP;
}
