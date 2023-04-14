#include "./SpiCmdInfoClass.h"
#include "PrivateEventClass.h"

PrivateEventClass::PrivateEventClass( EventType_e pEventType, DataType_e pDataType, std::string pStr )
{
    mEventType_e = pEventType;
    mDataType_e = pDataType;

    mInfoStr = pStr;

    mFreeFlag = NoFreeType;
}


PrivateEventClass::PrivateEventClass( EventType_e pEventType, DataType_e pDataType, Sender_e pSender, void * pParamP )
{
    mEventType_e = pEventType;
    mDataType_e = pDataType;
    mSender_e = pSender;

    mVoidParam1P = pParamP;

    mFreeFlag = NoFreeType;
}


PrivateEventClass::PrivateEventClass( EventType_e pEventType, DataType_e pDataType, Sender_e pSender, void * pParamP, int pLen, int pStartOffset )
{
    mEventType_e = pEventType;
    mDataType_e = pDataType;
    mSender_e = pSender;

    mVoidParam1P = pParamP;
    mVoidParam1Len = pLen;

    mIntParam1 = pStartOffset;

    mFreeFlag = Uint8ArrayType;
}

PrivateEventClass::PrivateEventClass( EventType_e pEventType, DataType_e pDataType, Sender_e pSender, void * pParamP, void * pParam1P )
{
    mEventType_e = pEventType;
    mDataType_e = pDataType;
    mSender_e = pSender;

    mVoidParam1P = pParamP;
    mVoidParam2P = pParam1P;

    mFreeFlag = NoFreeType;
}


PrivateEventClass::~PrivateEventClass()
{
    if( mFreeFlag == FreeParamType_e::SpiCmdInfoClassType )
    {
        delete ( SpiCmdInfoClass * )mVoidParam1P;
    }
    else if( mFreeFlag == FreeParamType_e::Uint8ArrayType )
    {
        uint8_t * _tmpP;

        _tmpP = ( uint8_t * )mVoidParam1P;

        delete [] _tmpP;
    }
}
