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
int fun_BsDiffRestore( uint8_t * pOldDataP, int pOldLen, uint8_t * pBsDataP, int pBsLen, int pStartFlag )
{
    int _retValue;
    int _tmpLen;
    int _tmpOffset;
    uint8_t * _tmpP;
    off_t _tmpCtrl[ 3 ];

    _retValue = 0;

    if( pStartFlag == 0 )
    {
        if( mBsDiffRestoreP != NULL )
        {
            if( mBsDiffRestoreP->BsNewFileBufP != NULL )
            {
                free( mBsDiffRestoreP->BsNewFileBufP );
            }
            if( mBsDiffRestoreP->mCtrlBufP != NULL )
            {
                free( mBsDiffRestoreP->mCtrlBufP );
            }
            if( mBsDiffRestoreP->mDataBufP != NULL )
            {
                free( mBsDiffRestoreP->mDataBufP );
            }
            if( mBsDiffRestoreP->mEDataBufP != NULL )
            {
                free( mBsDiffRestoreP->mEDataBufP );
            }
            free( mBsDiffRestoreP );
        }

        mBsDiffRestoreP = ( BsDiffRestore_S * )malloc( sizeof( BsDiffRestore_S ) );
        memset( mBsDiffRestoreP, 0, sizeof( BsDiffRestore_S ) );
        mBsDiffRestoreP->mBsFileDataP = pBsDataP;
        mBsDiffRestoreP->mBsFileLen = pBsLen;
        mBsDiffRestoreP->mOldDataP = pOldDataP;
        mBsDiffRestoreP->mOldLen = pOldLen;

        _tmpP = pBsDataP;

        mBsDiffRestoreP->BsCtrlLen = offtin( _tmpP + 8 );
        mBsDiffRestoreP->BsDataLen = offtin( _tmpP + 16 );
        mBsDiffRestoreP->BsNewFileLen = offtin( _tmpP + 24 );

        mBsDiffRestoreP->BsNewFileBufP = malloc( mBsDiffRestoreP->BsNewFileLen + 1);
        memset( mBsDiffRestoreP->BsNewFileBufP, 0, mBsDiffRestoreP->BsNewFileLen + 1 );

        mBsDiffRestoreP->mState = ReadAllLenState;

        _retValue = 2;

    }
    else
    {
        if( mBsDiffRestoreP->mState == ReadAllLenState )
        {
            mBsDiffRestoreP->mCtrlBufP = malloc( mBsDiffRestoreP->BsCtrlLen );
            memcpy( mBsDiffRestoreP->mCtrlBufP, &mBsDiffRestoreP->mBsFileDataP[ 32 ], mBsDiffRestoreP->BsCtrlLen );
            mBsDiffRestoreP->mDataBufP = malloc( mBsDiffRestoreP->BsDataLen );
            memcpy( mBsDiffRestoreP->mDataBufP, &mBsDiffRestoreP->mBsFileDataP[ 32 + mBsDiffRestoreP->BsCtrlLen ], mBsDiffRestoreP->BsDataLen );

            _tmpOffset = 32 + mBsDiffRestoreP->BsCtrlLen + mBsDiffRestoreP->BsDataLen;
            _tmpLen = mBsDiffRestoreP->mBsFileLen - 32 - mBsDiffRestoreP->BsCtrlLen - mBsDiffRestoreP->BsDataLen;

            mBsDiffRestoreP->mEDataBufP = malloc( _tmpLen );
            memcpy( mBsDiffRestoreP->mEDataBufP, &mBsDiffRestoreP->mBsFileDataP[ _tmpOffset ], _tmpLen );

            mBsDiffRestoreP->mState = BufReadyOkState;

            _retValue = 2;
        }
        else if( mBsDiffRestoreP->mState == BufReadyOkState )
        {
            mBsDiffRestoreP->oldPos = 0;
            mBsDiffRestoreP->newPos = 0;
            mBsDiffRestoreP->mCtrlBufOffset = 0;
            mBsDiffRestoreP->mDiffBufOffset = 0;
            mBsDiffRestoreP->mEdBufOffset = 0;

            mBsDiffRestoreP->mState = CtrlRunState;
            _retValue = 2;
        }
        else if( mBsDiffRestoreP->mState == CtrlRunState )
        {
            _retValue = 2;

            if( mBsDiffRestoreP->newPos < mBsDiffRestoreP->BsNewFileLen )
            {
                for( _tmpLen = 0; _tmpLen <= 2; _tmpLen++ )
                {
                    if( ( mBsDiffRestoreP->BsCtrlLen - mBsDiffRestoreP->mCtrlBufOffset ) < 8 )
                    {
                        _retValue = 0;
                        break;
                    }
                    _tmpCtrl[ _tmpLen ] = offtin( &mBsDiffRestoreP->mCtrlBufP[ mBsDiffRestoreP->mCtrlBufOffset ] );
                    mBsDiffRestoreP->mCtrlBufOffset += 8;

                }

                if( _retValue == 2 )
                {
                    if( ( mBsDiffRestoreP->newPos + _tmpCtrl[ 0 ] ) > mBsDiffRestoreP->BsNewFileLen )
                    {
                        _retValue = 0;
                    }
                    else
                    {
                        memcpy( &mBsDiffRestoreP->BsNewFileBufP[ mBsDiffRestoreP->newPos ],
                               &mBsDiffRestoreP->mDataBufP[ mBsDiffRestoreP->mDiffBufOffset ],
                               _tmpCtrl[ 0 ] );
                        mBsDiffRestoreP->mDiffBufOffset += _tmpCtrl[ 0 ];
                        for( _tmpLen = 0; _tmpLen < _tmpCtrl[0]; _tmpLen++ )
                        {
                            mBsDiffRestoreP->BsNewFileBufP[ mBsDiffRestoreP->newPos + _tmpLen ] += mBsDiffRestoreP->mOldDataP[ mBsDiffRestoreP->oldPos + _tmpLen ];
                        }

                        mBsDiffRestoreP->newPos += _tmpCtrl[ 0 ];
                        mBsDiffRestoreP->oldPos += _tmpCtrl[ 0 ];


                        memcpy( &mBsDiffRestoreP->BsNewFileBufP[ mBsDiffRestoreP->newPos ],
                               &mBsDiffRestoreP->mEDataBufP[ mBsDiffRestoreP->mEdBufOffset ],
                               _tmpCtrl[ 1 ] );
                        mBsDiffRestoreP->mEdBufOffset += _tmpCtrl[ 1 ];
                        mBsDiffRestoreP->newPos += _tmpCtrl[ 1 ];
                        mBsDiffRestoreP->oldPos += _tmpCtrl[ 2 ];

                    }
                }
            }
            else
            {
                _retValue = 1;
            }

        }
    }

    return _retValue;
}

/**
 * @brief fun_GetBsDiffRestoreState
 * @return
 */
BsRestoreState_e fun_GetBsDiffRestoreState( void )
{
    BsRestoreState_e _retState;

    _retState = NoRestoreState;

    if( mBsDiffRestoreP != NULL )
    {
        _retState = mBsDiffRestoreP->mState;
    }

    return _retState;
}

/**
 * @brief fun_GetBsDiffRestoreBuf
 * @param pRetLen
 * @return
 */
uint8_t * fun_GetBsDiffRestoreBuf( int pFlag, int * pRetLen )
{
    uint8_t * _retP;

    _retP = NULL;

    if( mBsDiffRestoreP->mState == BufReadyOkState )
    {
        if( pFlag == 0 )
        {
            _retP = mBsDiffRestoreP->mCtrlBufP;
            *pRetLen = mBsDiffRestoreP->BsCtrlLen;
        }
        else if( pFlag == 1 )
        {
            _retP = mBsDiffRestoreP->mDataBufP;
            *pRetLen = mBsDiffRestoreP->BsDataLen;
        }
    }
    else if( mBsDiffRestoreP->mState == CtrlRunState )
    {
        _retP = mBsDiffRestoreP->BsNewFileBufP;
        *pRetLen = mBsDiffRestoreP->BsNewFileLen;
    }

    return _retP;
}
