#include "PrivateEventClass.h"

PrivateEventClass::PrivateEventClass( EventType_e pEventType, DataType_e pDataType, std::string pStr )
{
    mEventType_e = pEventType;
    mDataType_e = pDataType;

    mInfoStr = pStr;
    mFreeFlag = 0;
}


PrivateEventClass::PrivateEventClass( EventType_e pEventType, DataType_e pDataType, Sender_e pSender, void * pParamP )
{
    mEventType_e = pEventType;
    mDataType_e = pDataType;
    mSender_e = pSender;

    mVoidParam1P = pParamP;
}


PrivateEventClass::~PrivateEventClass()
{
    if( mFreeFlag == 1 )
    {
        delete mVoidParam1P;
    }
    else if( mFreeFlag == 2 )
    {
        delete [] mVoidParam1P;
    }
}
