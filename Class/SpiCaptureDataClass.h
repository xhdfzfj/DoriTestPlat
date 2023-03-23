#ifndef SPICAPTUREDATACLASS_H
#define SPICAPTUREDATACLASS_H

#include <string>
#include <functional>
#include <thread>
#include "./FileAnalyseBaseClass.h"
#include "./SpiCmdInfoClass.h"

typedef struct __spiCmdStruct_S
{
    uint8_t mCmd;
    std::string mCmdDescribeStr;
    int mByteS;     //预期的字节数 -1代表没有预期
}spiCmdStruct_S;

class SpiCaptureDataClass : FileAnalyseBaseClass, SelfClearInterFaceClass
{
public:
    SpiCaptureDataClass( string pFilePath );
    virtual ~SpiCaptureDataClass();
    void sub_SetParentEventInterface( std::function< void( void * ) > pParentEventInf ) { mParentEventInf = pParentEventInf; }

    void sub_StartAnalyse();

protected:
    void sub_ClearHandle() override;
    void sub_SpiDataAnalyseHandle();

private:
    spiCmdStruct_S * fun_FindSpiCmdInArray( uint8_t pData );
    int fun_SpiCmdAnalyse( uint8_t * pSpiCmdDataP, int pDataLen, spiCmdStruct_S * pDestSpiCmdStructP, int pStartOffsetInFile );
    void sub_GetSpiReadData( uint8_t * pSaveBufP, uint8_t * pSpiCmdStartP, uint16_t pLen );
    //void sub_Test() { mParentEventInf( nullptr ); }

private:
    uint8_t * mSpiDataBufP;
    int mSpiDataBufLen;
    std::function< void( void * ) > mParentEventInf;

    bool mActionFlag;
    bool mAnalyseBreakFlag;
    std::thread * mAnalyseThreadP;

    /***************************
     * 以下调试用
     * ************************/
private:
    bool DebugStopFlag = false;
};

#endif // SPICAPTUREDATACLASS_H
