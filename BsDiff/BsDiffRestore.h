#ifndef BSDIFFRESTORE_H
#define BSDIFFRESTORE_H

#include <stdint.h>

typedef struct __BsDiffRestore_S
{
    uint8_t * mOldDataP;
    int mOldLen;
    uint8_t * mBsDataP;
    int mBsLen;

    ssize_t BsCtrlLen;
    ssize_t BsDataLen;
    ssize_t BsNewFileLen;
}BsDiffRestore_S;

#if __cplusplus
extern "C" {
#endif

int fun_BsDiffRestore( uint8_t * pOldDataP, int pOldLen, uint8_t * pBsDataP, int pBsLen );

#if __cplusplus
}
#endif

#endif // BSDIFFRESTORE_H
