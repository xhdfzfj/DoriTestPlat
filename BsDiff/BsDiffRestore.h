#ifndef BSDIFFRESTORE_H
#define BSDIFFRESTORE_H

#include <stdlib.h>
#include <stdint.h>

typedef enum _BsRestoreState_e
{
    NoRestoreState,
    ReadAllLenState,
    BufReadyOkState,
    CtrlRunState,
}BsRestoreState_e;

typedef struct __BsDiffRestore_S
{
    uint8_t * mOldDataP;
    int mOldLen;
    uint8_t * mBsFileDataP;
    int mBsFileLen;

    BsRestoreState_e mState;

    ssize_t BsCtrlLen;
    ssize_t BsDataLen;
    ssize_t BsNewFileLen;

    uint8_t * mCtrlBufP;
    uint8_t * mDataBufP;
    uint8_t * mEDataBufP;

    uint8_t * BsNewFileBufP;

    int oldPos;
    int newPos;
    int mCtrlBufOffset;
    int mDiffBufOffset;
    int mEdBufOffset;

}BsDiffRestore_S;

#if __cplusplus
extern "C" {
#endif

int fun_BsDiffRestore( uint8_t * pOldDataP, int pOldLen, uint8_t * pBsDataP, int pBsLen, int pStartFlag );
BsRestoreState_e fun_GetBsDiffRestoreState( void );
uint8_t * fun_GetBsDiffRestoreBuf( int pFlag, int * pRetLen );

#if __cplusplus
}
#endif

#endif // BSDIFFRESTORE_H
