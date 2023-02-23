#ifndef PRIVATEEVENTCLASS_H
#define PRIVATEEVENTCLASS_H

#include "../Model/xhdlogeventclass.h"

typedef enum __EventType_e
{
    logInfoType,
    SpiCmdInfoType,
}EventType_e;

typedef enum __DataType_e
{
    StringType,
    DataType,
}DataType_e;

typedef enum __Sender_e
{
    SpiCapture,
}Sender_e;

class PrivateEventClass
{
public:
    PrivateEventClass( EventType_e pEventType, DataType_e pDataType, std::string pStr );
    PrivateEventClass( EventType_e pEventType, DataType_e pDataType, Sender_e pSender, void * pParamP );
    virtual ~PrivateEventClass();

    void SetLogLevel( xhdLogEventClass::LogLevel pLevel ) { mLoglevel = pLevel; }
    void SetSender( Sender_e pSender ) { mSender_e = pSender; }

public:
    EventType_e mEventType_e;
    xhdLogEventClass::LogLevel mLoglevel;
    DataType_e mDataType_e;
    Sender_e mSender_e;

    void * mVoidParam1P;

    std::string mInfoStr;

private:
    int mFreeFlag;  // 0 不用释放 mVoidParm1P
                    // 1 释放 mVoidParam1P
                    // 2 以数组方式进行释放 mVoidParam1P

};

#endif // PRIVATEEVENTCLASS_H
