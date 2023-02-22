#ifndef SPICAPTUREDATACLASS_H
#define SPICAPTUREDATACLASS_H

#include <string>
#include <functional>
#include <thread>
#include "./FileAnalyseBaseClass.h"

typedef struct __spiCmdStruct_S
{
    uint8_t mCmd;
    std::string mCmdDescribeStr;
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
    //void sub_Test() { mParentEventInf( nullptr ); }

private:
    uint8_t * mSpiDataBufP;
    int mSpiDataBufLen;
    std::function< void( void * ) > mParentEventInf;

    bool mActionFlag;
    bool mAnalyseBreakFlag;
    std::thread * mAnalyseThreadP;
};

#endif // SPICAPTUREDATACLASS_H
