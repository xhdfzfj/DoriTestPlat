#ifndef SPICMDINFOCLASS_H
#define SPICMDINFOCLASS_H

#include <string>

class SpiCmdInfoClass
{
public:
    SpiCmdInfoClass( std::string pCmdInfo, int pCmd, uint8_t * pDataP, int pDataLen );
    virtual ~SpiCmdInfoClass();

    SpiCmdInfoClass * Clone();

public:
    std::string mCmdInfo;
    int mCmd;
    uint8_t * mDataP;
    int mDataLen;
};

#endif // SPICMDINFOCLASS_H
