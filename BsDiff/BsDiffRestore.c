#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./BsDiffRestore.h"

typedef unsigned char   u_char;

BsDiffRestore_S * mBsDiffRestoreP = NULL;

off_t offtin(u_char *buf)
{
    off_t y;

    y=buf[7]&0x7F;
    y=y*256;y+=buf[6];
    y=y*256;y+=buf[5];
    y=y*256;y+=buf[4];
    y=y*256;y+=buf[3];
    y=y*256;y+=buf[2];
    y=y*256;y+=buf[1];
    y=y*256;y+=buf[0];

    if(buf[7]&0x80) y=-y;

    return y;
}

/**
 * @brief fun_BsDiffRestore
 *      处理BsDiff生成的文件
 * @param pOldDataP
 * @param pOldLen
 * @param pBsDataP
 * @param pBsLen
 * @return
 *      0 代表出错
 *      1 代表完成
 *      2 代表可以继续进行
 */
int fun_BsDiffRestore( uint8_t * pOldDataP, int pOldLen, uint8_t * pBsDataP, int pBsLen )
{
    int _retValue;
    uint8_t * _tmpP;

    _retValue = 0;

    if( mBsDiffRestoreP == NULL )
    {
        mBsDiffRestoreP = ( BsDiffRestore_S * )malloc( sizeof( BsDiffRestore_S ) );
        mBsDiffRestoreP->mBsDataP = pBsDataP;
        mBsDiffRestoreP->mBsLen = pBsLen;
        mBsDiffRestoreP->mOldDataP = pOldDataP;
        mBsDiffRestoreP->mOldLen = pOldLen;

        _tmpP = pBsDataP;

        mBsDiffRestoreP->BsCtrlLen = offtin( _tmpP + 8 );
        mBsDiffRestoreP->BsDataLen = offtin( _tmpP + 16 );
        mBsDiffRestoreP->BsNewFileLen = offtin( _tmpP + 24 );

        _retValue = 2;
    }
    else
    {

    }

    return _retValue;
}
