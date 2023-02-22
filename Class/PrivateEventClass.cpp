#include "PrivateEventClass.h"

PrivateEventClass::PrivateEventClass( EventType_e pEventType, DataType_e pDataType, std::string pStr )
{
    mEventType_e = pEventType;
    mDataType_e = pDataType;

    mInfoStr = pStr;
}


PrivateEventClass::~PrivateEventClass()
{

}
