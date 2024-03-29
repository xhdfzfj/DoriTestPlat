﻿#include <cstring>
#include "SpiCmdInfoClass.h"

SpiCmdInfoClass::SpiCmdInfoClass( std::string pCmdInfo, int pCmd, uint8_t * pDataP, int pDataLen )
{
    mCmdInfo = pCmdInfo;
    mCmd = pCmd;
    mDataLen = pDataLen;

    mDataP = pDataP;

    mStartOffsetInFile = -1;
    mOperateLen = -1;
}


SpiCmdInfoClass::~SpiCmdInfoClass()
{
    if( mDataP != nullptr )
    {
        delete [] mDataP;
    }
}

SpiCmdInfoClass * SpiCmdInfoClass::Clone( void )
{
    SpiCmdInfoClass * _retObj;

    _retObj = new SpiCmdInfoClass( mCmdInfo, mCmd, nullptr, 0 );
    _retObj->mStartOffsetInFile = mStartOffsetInFile;
    _retObj->mOperateLen = mOperateLen;

    uint8_t * _tmpP;

    if( mDataP != nullptr )
    {
        _tmpP = new uint8_t [ mDataLen ];

        memmove( _tmpP, mDataP, mDataLen );
        _retObj->mDataLen = mDataLen;
        _retObj->mDataP = _tmpP;
    }

    return _retObj;
}
