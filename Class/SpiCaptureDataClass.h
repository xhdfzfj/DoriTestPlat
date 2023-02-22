#ifndef SPICAPTUREDATACLASS_H
#define SPICAPTUREDATACLASS_H

#include <string>

#include "./FileAnalyseBaseClass.h"

typedef struct __spiCmdStruct_S
{
    uint8_t mCmd;
    std::string mCmdDescribeStr;
}spiCmdStruct_S;

class SpiCaptureDataClass : FileAnalyseBaseClass
{
public:
    SpiCaptureDataClass( string pFilePath );
    virtual ~SpiCaptureDataClass();

private:
    uint8_t * mSpiDataBufP;
    int mSpiDataBufLen;
};

#endif // SPICAPTUREDATACLASS_H
