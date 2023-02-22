#ifndef PRIVATEEVENTCLASS_H
#define PRIVATEEVENTCLASS_H

#include "../Model/xhdlogeventclass.h"

typedef enum __EventType_e
{
    logInfoType,
}EventType_e;

typedef enum __DataType_e
{
    StringType,
    DataType,
}DataType_e;

class PrivateEventClass
{
public:
    PrivateEventClass( EventType_e pEventType, DataType_e pDataType, std::string pStr );
    virtual ~PrivateEventClass();

    void SetLogLevel( xhdLogEventClass::LogLevel pLevel ) { mLoglevel = pLevel; }

public:
    EventType_e mEventType_e;
    xhdLogEventClass::LogLevel mLoglevel;
    DataType_e mDataType_e;

    std::string mInfoStr;

};

#endif // PRIVATEEVENTCLASS_H
