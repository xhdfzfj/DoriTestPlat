﻿#ifndef PRIVATEEVENTCLASS_H
#define PRIVATEEVENTCLASS_H

#include "../Model/xhdlogeventclass.h"

typedef enum __EventType_e
{
    logInfoType,
    SpiCmdInfoType,
    HexDataDisplayType,
    ReDrawHexData,
    GuiTransionEffect,
    ModifySimFlashContent,
}EventType_e;

typedef enum __DataType_e
{
    StringType,
    DataType,
}DataType_e;

typedef enum __Sender_e
{
    SpiCapture,
    TransionEffector,
    SimFlash,
}Sender_e;

typedef enum __FreeParamType_e
{
    NoFreeType,
    SpiCmdInfoClassType,
    Uint8ArrayType,
    StructPointType,
}FreeParamType_e;

typedef struct __FlashModifyContent_s
{
    int mStartAddress;      //开始的地址一定是8字节对齐
    int mStartOffset;       //从8字节对齐后的偏移
    int mLineByteCount;     //每行选中的字节数
    int mLineCount;         //有几行选中
    int mGuiLineDisplayByteCount;    //GUI上每行显示的个数
}FlashModifyContent_s;

class PrivateEventClass
{
public:
    PrivateEventClass( EventType_e pEventType, DataType_e pDataType, std::string pStr );
    PrivateEventClass( EventType_e pEventType, DataType_e pDataType, Sender_e pSender, void * pParamP );
    PrivateEventClass( EventType_e pEventType, DataType_e pDataType, Sender_e pSender, void * pParamP, void * pParam1P );
    PrivateEventClass( EventType_e pEventType, DataType_e pDataType, Sender_e pSender, void * pParamP, int pLen, int pStartOffset );
    virtual ~PrivateEventClass();

    void SetLogLevel( xhdLogEventClass::LogLevel pLevel ) { mLoglevel = pLevel; }
    void SetSender( Sender_e pSender ) { mSender_e = pSender; }
    void SetFreeState( FreeParamType_e pFreeState ) { mFreeFlag = pFreeState; }

public:
    EventType_e mEventType_e;
    xhdLogEventClass::LogLevel mLoglevel;
    DataType_e mDataType_e;
    Sender_e mSender_e;

    void * mVoidParam1P;
    int mVoidParam1Len;

    int mIntParam1;

    std::string mInfoStr;

    void * mVoidParam2P;

private:
    FreeParamType_e mFreeFlag;
};

#endif // PRIVATEEVENTCLASS_H
