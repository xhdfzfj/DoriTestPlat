#pragma once

#include <stdlib.h>
#include <stdint.h>

typedef struct __TreeNode_S
{
    void * mLeftChildP;
    void * mRightChildP;
    uint8_t mVirtualFlag;      //虚拟标志
    void * mValueP;
}TreeNode_S;



#if __cplusplus
extern "C" {
#endif

uint8_t * fun_CreateHuffmanCode( uint8_t * pDestDataP, uint32_t pDestDataLen, int * pRetLen );

#if __cplusplus
}
#endif
