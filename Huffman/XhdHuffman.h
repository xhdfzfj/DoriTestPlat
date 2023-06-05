#pragma once

#include "../Include/CommonStruct.h"

typedef struct __HuffmanResult_S
{
    uint32_t mDestValue;
    uint8_t mBitS;
    uint8_t mHuffmanValue[ 16 ];
}HuffmanResult_S;

#if __cplusplus
extern "C" {
#endif

HuffmanResult_S * fun_CreateHuffmanCode( uint8_t * pDestDataP, uint32_t pDestDataLen, int * pRetLen );

#if __cplusplus
}
#endif
