﻿#include "memory.h"
#include "./XhdHuffman.h"
#include <math.h>
#include <stdio.h>

int min(int x, int y) 
{
    return x < y ? x : y;
}

void sub_MergSort( uint32_t * pSortBufP, uint32_t pLen )
{
    uint32_t * _tmpSortBuf;
    uint32_t * _tmpParamBufP;
    int seg, start;

    _tmpParamBufP = pSortBufP;
    _tmpSortBuf = ( uint32_t * )malloc( sizeof( uint32_t ) * pLen );
    for( seg = 1; seg < pLen; seg += seg )
    {
        for( start = 0; start < pLen; start += ( seg + seg ) )
        {
            int low = start;
            int mid = min( start + seg, pLen );
            int high = min( start + seg + seg, pLen );
            int start1 = low;
            int end1 = mid;
            int start2 = mid;
            int end2 = high;
            int k = low;

            printf( "start1 %d end1 %d, start2 %d end2 %d\r\n", start1, end1, start2, end2 );

            while( ( start1 < end1 ) && ( start2 < end2 ) )
            {
                _tmpSortBuf[ k++ ] = _tmpParamBufP[ start1 ] < _tmpParamBufP[ start2 ] ? _tmpParamBufP[ start1++ ] : _tmpParamBufP[ start2++ ];
            }
            while( start1 < end1 )
            {
                _tmpSortBuf[ k++ ] = _tmpParamBufP[ start1++ ];
            }
            while( start2 < end2 )
            {
                _tmpSortBuf[ k++ ] = _tmpParamBufP[ start2++ ];
            }
        }

        uint32_t * _tmpP;
        _tmpP = _tmpParamBufP;
        _tmpParamBufP = _tmpSortBuf;
        _tmpSortBuf = _tmpP;
    }

    if( _tmpParamBufP != pSortBufP )
    {
        memcpy( _tmpSortBuf, _tmpParamBufP, sizeof( uint32_t ) * pLen );
        _tmpSortBuf = _tmpParamBufP;
    }

    free( _tmpSortBuf );
}


TreeNode_S * fun_CreateHuffmanTree( uint32_t * pDestP, uint32_t pElementCount )
{
    TreeNode_S * _retNodeP;
    TreeNode_S * _tmpLeaf1NodeP;
    TreeNode_S * _tmpLeaf2NodeP;
    TreeNode_S * _tmpNodeP;
    TreeNode_S * _tmpNode1P;
    TreeNode_S * _tmpNode2P;
    uint32_t _tmpU32Value, _tmpU32Value1;
    uint32_t i;
    uint8_t _flag;

    _retNodeP = NULL;
    i = 0;
    if( pElementCount > 1 )
    {
        _tmpLeaf2NodeP = ( TreeNode_S * )malloc( sizeof( TreeNode_S ) );
        _tmpLeaf1NodeP = ( TreeNode_S * )malloc( sizeof( TreeNode_S ) );
        _tmpNodeP = ( TreeNode_S * )malloc( sizeof( TreeNode_S ) );
        INIT_TREENODE( _tmpLeaf1NodeP );
        INIT_TREENODE( _tmpLeaf2NodeP );
        INIT_TREENODE( _tmpNodeP );

        _tmpLeaf1NodeP->mValue_U.mVoidPointP = ( void * )( &pDestP[ 0 ] );
        _tmpLeaf2NodeP->mValue_U.mVoidPointP = ( void * )( &pDestP[ 1 ] );
        _tmpLeaf1NodeP->mParentP = ( void * )_tmpNodeP;
        _tmpLeaf2NodeP->mParentP = ( void * )_tmpNodeP;

        _tmpNodeP->mVirtualFlag = 1;
        _tmpNodeP->mValue_U.mUint32Value = ( pDestP[ 0 ] + pDestP[ 1 ] );

        _tmpNodeP->mLeftChildP = ( void * )_tmpLeaf1NodeP;
        _tmpNodeP->mRightChildP = ( void * )_tmpLeaf2NodeP;
        i += 2;
        while( i < pElementCount )
        {
            _tmpU32Value1 = _tmpNodeP->mValue_U.mUint32Value;
            _tmpU32Value = pDestP[ i ];
            i += 1;

            _flag = 0;
            if( i < pElementCount )
            {
                _tmpU32Value += pDestP[ i ];
                i += 1;
                _flag = 1;

                if( abs( ( int )( _tmpU32Value1 - _tmpU32Value ) ) >
                    abs( ( int )( _tmpU32Value1 - _tmpU32Value + pDestP[ i - 1 ] ) ) )
                {
                    i -= 1;
                    _flag = 0;
                }
            }

            if( _flag == 0 )
            {
                _tmpNode1P = _tmpNodeP;
                _tmpLeaf1NodeP = ( TreeNode_S * )malloc( sizeof( TreeNode_S ) );
                _tmpNodeP = ( TreeNode_S * )malloc( sizeof( TreeNode_S ) );
                INIT_TREENODE( _tmpLeaf1NodeP );
                INIT_TREENODE( _tmpNodeP );

                _tmpLeaf1NodeP->mValue_U.mVoidPointP = ( void * )( &pDestP[ i - 1 ] );
                _tmpLeaf1NodeP->mParentP = _tmpNodeP;
                _tmpNode1P->mParentP = _tmpNodeP;
                _tmpNodeP->mVirtualFlag = 1;
                _tmpNodeP->mLeftChildP = ( void * )_tmpNode1P;
                _tmpNodeP->mRightChildP = ( void * )_tmpLeaf1NodeP;
                _tmpNodeP->mValue_U.mUint32Value = _tmpU32Value1 + pDestP[ i - 1 ];
            }
            else
            {
                _tmpNode1P = _tmpNodeP;

                _tmpLeaf2NodeP = ( TreeNode_S * )malloc( sizeof( TreeNode_S ) );
                _tmpLeaf1NodeP = ( TreeNode_S * )malloc( sizeof( TreeNode_S ) );
                _tmpNodeP = ( TreeNode_S * )malloc( sizeof( TreeNode_S ) );
                INIT_TREENODE( _tmpLeaf1NodeP );
                INIT_TREENODE( _tmpLeaf2NodeP );
                INIT_TREENODE( _tmpNodeP );

                _tmpLeaf1NodeP->mValue_U.mVoidPointP = ( void * )( &pDestP[ i - 2 ] );
                _tmpLeaf2NodeP->mValue_U.mVoidPointP = ( void * )( &pDestP[ i - 1 ] );
                _tmpLeaf1NodeP->mParentP = ( void * )_tmpNodeP;
                _tmpLeaf2NodeP->mParentP = ( void * )_tmpNodeP;

                _tmpNodeP->mVirtualFlag = 1;
                _tmpNodeP->mValue_U.mUint32Value = ( pDestP[ i - 2 ] + pDestP[ i - 1 ] );

                _tmpNodeP->mLeftChildP = ( void * )_tmpLeaf1NodeP;
                _tmpNodeP->mRightChildP = ( void * )_tmpLeaf2NodeP;

                _tmpNode2P = ( TreeNode_S * )malloc( sizeof( TreeNode_S ) );
                INIT_TREENODE( _tmpNode2P );
                _tmpNode2P->mVirtualFlag = 1;
                _tmpNode2P->mLeftChildP = _tmpNode1P;
                _tmpNode2P->mRightChildP = _tmpNodeP;
                _tmpNode1P->mParentP = ( void * )_tmpNode2P;
                _tmpNodeP->mParentP = ( void * )_tmpNode2P;
                _tmpNode2P->mValue_U.mUint32Value = _tmpNodeP->mValue_U.mUint32Value + _tmpNode1P->mValue_U.mUint32Value;
                _tmpNodeP = _tmpNode2P;
            }
        }

        _retNodeP = _tmpNodeP;
    }
    return _retNodeP;
}

void sub_DisplayTreeLeafNode( TreeNode_S * pRootNodeP )
{
    Stack_S * _tmpStackP;
    ListElement_S * _tmpElementP;
    TreeNode_S * _tmpNodeP;
    uint32_t _tmpU32Value;

    if( pRootNodeP != NULL )
    {
        _tmpStackP = ( Stack_S * )malloc( sizeof( Stack_S ) );
        INIT_STACK( _tmpStackP );

        _tmpNodeP = pRootNodeP;

        while( ( _tmpNodeP != NULL ) && ( _tmpNodeP->mLeftChildP == NULL ) )
        {
            if( ( _tmpNodeP->mLeftChildP == NULL ) && ( _tmpNodeP->mRightChildP != NULL ) )
            {
                _tmpU32Value = *( ( uint32_t * )_tmpNodeP->mValue_U.mVoidPointP );
                printf( "leaf is %d\r\n", _tmpU32Value );
            }
            else
            {
                _tmpU32Value = _tmpNodeP->mValue_U.mUint32Value;
                printf( "node is %d\r\n", _tmpU32Value );
            }
            _tmpNodeP = _tmpNodeP->mRightChildP;
        }

        if( _tmpNodeP != NULL )
        {
            //找到第一个左子不为NULL的节点
            _tmpElementP = ( ListElement_S * )malloc( sizeof( ListElement_S ) );
            INIT_LISTELEMENT( _tmpElementP );
            _tmpElementP->ElementP = ( void * )_tmpNodeP;

            PUSH_STACK( _tmpStackP, _tmpElementP );
            _tmpNodeP = _tmpNodeP->mLeftChildP;

            while( ( _tmpStackP->ElementCount != 0 ) || ( _tmpNodeP != NULL ) )
            {
                while( ( _tmpNodeP != NULL ) && ( _tmpNodeP->mLeftChildP == NULL ) )
                {
                    if( ( _tmpNodeP->mLeftChildP == NULL ) && ( _tmpNodeP->mRightChildP == NULL ) )
                    {
                        _tmpU32Value = *( ( uint32_t * )_tmpNodeP->mValue_U.mVoidPointP );
                        printf( "leaf is %d\r\n", _tmpU32Value );
                    }
                    else
                    {
                        _tmpU32Value = _tmpNodeP->mValue_U.mUint32Value;
                        printf( "node is %d\r\n", _tmpU32Value );
                    }
                    _tmpNodeP = _tmpNodeP->mRightChildP;
                }

                if( _tmpNodeP != NULL )
                {
                    _tmpElementP = ( ListElement_S * )malloc( sizeof( ListElement_S ) );
                    INIT_LISTELEMENT( _tmpElementP );
                    _tmpElementP->ElementP = ( void * )_tmpNodeP;
                    PUSH_STACK( _tmpStackP, _tmpElementP );

                    _tmpNodeP = _tmpNodeP->mLeftChildP;
                    if( ( _tmpNodeP->mLeftChildP == NULL ) && ( _tmpNodeP->mRightChildP == NULL ) )
                    {
                        _tmpU32Value = *( ( uint32_t * )_tmpNodeP->mValue_U.mVoidPointP );
                        printf( "leaf is %d\r\n", _tmpU32Value );

                        POP_STACK( _tmpStackP, _tmpElementP );
                        _tmpNodeP = _tmpElementP->ElementP;
                        free( _tmpElementP );

                        _tmpU32Value = _tmpNodeP->mValue_U.mUint32Value;
                        printf( "node is %d\r\n", _tmpU32Value );
                        _tmpNodeP = _tmpNodeP->mRightChildP;
                        continue;
                    }

                }
                else if( _tmpStackP->ElementCount != 0 )
                {
                    POP_STACK( _tmpStackP, _tmpElementP );
                    _tmpNodeP = _tmpElementP->ElementP;
                    free( _tmpElementP );

                    _tmpU32Value = _tmpNodeP->mValue_U.mUint32Value;
                    printf( "node is %d\r\n", _tmpU32Value );
                    _tmpNodeP = _tmpNodeP->mRightChildP;

                    continue;
                }
            }
        }
    }
}

uint8_t * fun_CreateHuffmanCode( uint8_t * pDestDataP, uint32_t pDestDataLen, int * pRetLen )
{
    uint32_t i, _tmpLen;
    uint8_t * _retP;
    uint32_t _tmpValueS[ 256 ];
    uint32_t _tmpCodeS[ 256 ];

    _retP = NULL;
    memset( ( void * )_tmpCodeS, 0, sizeof( uint32_t ) * 256 );

    _tmpLen = pDestDataLen;

    for( i = 0; i < _tmpLen; i++ )
    {
        _tmpCodeS[ pDestDataP[ i ] ] += 1;
    }

    memcpy( _tmpValueS, _tmpCodeS, sizeof( uint32_t ) * 256 );

    sub_MergSort( _tmpValueS, 256 );
    for( i = 0; i < 256; i++ )
    {
        if( _tmpValueS[ i ] != 0 )
        {
            break;
        }
    }

    if( i != 256 )
    {
        //不为全空的情况
        _tmpLen = 256 - i;

        TreeNode_S * _tmpHuffmanRootP;

        _tmpHuffmanRootP = fun_CreateHuffmanTree( &_tmpValueS[ i ], _tmpLen );

        sub_DisplayTreeLeafNode( _tmpHuffmanRootP );
    }

    return _retP;
}
