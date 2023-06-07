#include "memory.h"
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

            //printf( "start1 %d end1 %d, start2 %d end2 %d\r\n", start1, end1, start2, end2 );

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

        free( _tmpStackP );
    }
}


HuffmanResult_S fun_GetHuffmanVaule( TreeNode_S * pRootNode, TreeNode_S * pDestNodeP )
{
    HuffmanResult_S _retValue;
    uint32_t _tmpU32Value;
    TreeNode_S * _tmpNodeP;
    int _tmpByteIndex;
    int _tmpBitIndex;

    _tmpU32Value = *( ( uint32_t * )pDestNodeP->mValue_U.mVoidPointP );
    _retValue.mDestValue = _tmpU32Value;

    _retValue.mBitS = 0;
    memset( _retValue.mHuffmanValue, 0, 16 );

    _tmpNodeP = pDestNodeP;
    _tmpBitIndex = 0;
    _tmpByteIndex = 0;

    while( _tmpNodeP->mParentP != NULL )
    {
        _tmpBitIndex = _retValue.mBitS & 0x07;
        if( ( ( TreeNode_S * )_tmpNodeP->mParentP )->mRightChildP == _tmpNodeP )
        {
            _retValue.mHuffmanValue[ _tmpByteIndex ] &= ( ~( 1 << _tmpBitIndex ) );
            _retValue.mHuffmanValue[ _tmpByteIndex ] |= ( 1 << _tmpBitIndex );
        }
        else
        {
            _retValue.mHuffmanValue[ _tmpByteIndex ] &= ( ~( 1 << _tmpBitIndex ) );
        }

        _retValue.mBitS += 1;
        _tmpByteIndex = _retValue.mBitS >> 3;

        _tmpNodeP = _tmpNodeP->mParentP;
    }

    return _retValue;
}


HuffmanResult_S * fun_GetHuffmanResultByHuffmanTree( TreeNode_S * pTreeRootNodeP, uint32_t pDestValueS )
{
    int i;
    HuffmanResult_S * _retResultArrayS;
    Stack_S * _tmpStackP;
    ListElement_S * _tmpElementP;
    TreeNode_S * _tmpNodeP;
    uint32_t _tmpU32Value;

    _retResultArrayS = NULL;

    if( pTreeRootNodeP != NULL )
    {
        i = 0;
        _tmpStackP = ( Stack_S * )malloc( sizeof( Stack_S ) );
        INIT_STACK( _tmpStackP );

        _retResultArrayS = ( HuffmanResult_S * )malloc( sizeof( HuffmanResult_S ) * pDestValueS );

        _tmpNodeP = pTreeRootNodeP;
        while( ( _tmpNodeP != NULL ) && ( _tmpNodeP->mLeftChildP == NULL ) )
        {
            if( ( _tmpNodeP->mLeftChildP == NULL ) && ( _tmpNodeP->mRightChildP != NULL ) )
            {
                _tmpU32Value = *( ( uint32_t * )_tmpNodeP->mValue_U.mVoidPointP );
                _retResultArrayS[ i ] = fun_GetHuffmanVaule( pTreeRootNodeP, _tmpNodeP );
                i += 1;
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
                        _retResultArrayS[ i ] = fun_GetHuffmanVaule( pTreeRootNodeP, _tmpNodeP );
                        i += 1;
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
                        _retResultArrayS[ i ] = fun_GetHuffmanVaule( pTreeRootNodeP, _tmpNodeP );
                        i += 1;

                        POP_STACK( _tmpStackP, _tmpElementP );
                        _tmpNodeP = _tmpElementP->ElementP;
                        free( _tmpElementP );

                        _tmpNodeP = _tmpNodeP->mRightChildP;
                        continue;
                    }

                }
                else if( _tmpStackP->ElementCount != 0 )
                {
                    POP_STACK( _tmpStackP, _tmpElementP );
                    _tmpNodeP = _tmpElementP->ElementP;
                    free( _tmpElementP );

                    _tmpNodeP = _tmpNodeP->mRightChildP;

                    continue;
                }
            }
        }

        free( _tmpStackP );
    }


    return _retResultArrayS;
}


void sub_ClearTree( TreeNode_S * pTreeNodeP )
{
    if( ( pTreeNodeP->mLeftChildP == NULL ) && ( pTreeNodeP->mRightChildP == NULL ) )
    {
        free( pTreeNodeP );
    }
    else
    {
        if( pTreeNodeP->mLeftChildP != NULL )
        {
            sub_ClearTree( ( TreeNode_S * )pTreeNodeP->mLeftChildP );
        }

        if( pTreeNodeP->mRightChildP != NULL )
        {
            sub_ClearTree( ( TreeNode_S * )pTreeNodeP->mRightChildP );
        }

        free( pTreeNodeP );
    }
}

uint8_t gBitMaskS[ 8 ] = { 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };
uint8_t gRervBitMaskS[ 8 ] = { 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff };

uint32_t fun_HuffmanCodeMapCreate( HuffmanResult_S * pHuffmanCodeP, uint32_t pHuffmanCodeLen, uint8_t * pRetBufP )
{
    uint8_t * _retP;
    uint8_t * _tmpP;
    uint32_t j, i, x, z;
    uint32_t _tmpLen;
    uint32_t _tmpByteIndex;
    uint8_t _tmpU8Value, _tmpU8Value1;
    uint8_t _tmpBitOffset;
    uint8_t _tmpHufByteIndex;
    uint8_t _tmpHufBitOffset;

    _retP = pRetBufP;

    j = 8;  //空出最前的

    _tmpBitOffset = 8;
    _tmpByteIndex = j;
    _retP[ _tmpByteIndex ] = 0;
    while( i < pHuffmanCodeLen )
    {
        if( ( _tmpLen - _tmpByteIndex ) < 32 )
        {
            _tmpLen += 256;
            _tmpP = ( uint8_t * )malloc( _tmpLen );
            memcpy( _tmpP, _retP, _tmpLen - 256 );
            free( _retP );

            _retP = _tmpP;
        }

        _tmpHufBitOffset = 8;
        do
        {
            z = 0;

            if( _tmpBitOffset < _tmpHufBitOffset )
            {
                z = _tmpHufBitOffset - _tmpBitOffset;
                _tmpHufBitOffset = _tmpBitOffset;
            }

            _tmpU8Value = _retP[ _tmpByteIndex ];
            //_tmpU8Value &= gRervBitMaskS[ _tmpBitOffset - 1 ];

            _tmpU8Value1 = pHuffmanCodeP[ i ].mDestValue;
            if( z )
            {
                _tmpU8Value1 &= gRervBitMaskS[ _tmpHufBitOffset - 1 ];
                _tmpU8Value |= ( _tmpU8Value1 >>  ( 8 - _tmpHufBitOffset ) );
            }
            else
            {
                _tmpU8Value1 &= gBitMaskS[ _tmpHufBitOffset - 1 ];
                _tmpU8Value |= ( _tmpU8Value1 << ( _tmpBitOffset - _tmpHufBitOffset ) );
            }

            _retP[ _tmpByteIndex ] = _tmpU8Value;

            _tmpBitOffset -= _tmpHufBitOffset;
            if( _tmpBitOffset == 0 )
            {
                _tmpByteIndex += 1;
                _tmpBitOffset = 8;
                _retP[ _tmpByteIndex ] = 0;
            }

            _tmpHufBitOffset = z;   //取出剩下的位数

        }while( z != 0 );

        _tmpHufBitOffset = 8;
        do
        {
            z = 0;

            if( _tmpBitOffset < _tmpHufBitOffset )
            {
                z = _tmpHufBitOffset - _tmpBitOffset;
                _tmpHufBitOffset = _tmpBitOffset;
            }

            _tmpU8Value = _retP[ _tmpByteIndex ];
            //_tmpU8Value &= gRervBitMaskS[ _tmpBitOffset - 1 ];

            _tmpU8Value1 = pHuffmanCodeP[ i ].mBitS;
            if( z )
            {
                _tmpU8Value1 &= gRervBitMaskS[ _tmpHufBitOffset - 1 ];
                _tmpU8Value |= ( _tmpU8Value1 >> ( 8 - _tmpHufBitOffset ) );
            }
            else
            {
                _tmpU8Value1 &= gBitMaskS[ _tmpHufBitOffset - 1 ];
                _tmpU8Value |= ( _tmpU8Value1 << ( _tmpBitOffset - _tmpHufBitOffset ) );
            }

            _retP[ _tmpByteIndex ] = _tmpU8Value;

            _tmpBitOffset -= _tmpHufBitOffset;
            if( _tmpBitOffset == 0 )
            {
                _tmpByteIndex += 1;
                _tmpBitOffset = 8;
                _retP[ _tmpByteIndex ] = 0;
            }

            _tmpHufBitOffset = z;   //取出剩下的位数

        }while( z != 0 );

        x = 0;

        _tmpHufByteIndex = pHuffmanCodeP[ i ].mBitS / 8;
        _tmpHufBitOffset = pHuffmanCodeP[ i ].mBitS % 8;

        do
        {
            z = 0;

            if( _tmpBitOffset < _tmpHufBitOffset )
            {
                z = _tmpHufBitOffset - _tmpBitOffset;
                _tmpHufBitOffset = _tmpBitOffset;
            }

            x += _tmpHufBitOffset;

            _tmpU8Value = _retP[ _tmpByteIndex ];
            //_tmpU8Value &= gRervBitMaskS[ _tmpBitOffset - 1 ];

            _tmpU8Value1 = pHuffmanCodeP[ i ].mHuffmanValue[ _tmpHufByteIndex ];
            if( z )
            {
                _tmpU8Value1 &= gRervBitMaskS[ _tmpHufBitOffset - 1 ];
                _tmpU8Value |= ( _tmpU8Value1 >> ( 8 - _tmpHufBitOffset ) );
            }
            else
            {
                _tmpU8Value1 &= gBitMaskS[ _tmpHufBitOffset - 1 ];
                _tmpU8Value |= ( _tmpU8Value1 << ( _tmpBitOffset - _tmpHufBitOffset ) );
            }
            _retP[ _tmpByteIndex ] = _tmpU8Value;

            _tmpBitOffset -= _tmpHufBitOffset;
            if( _tmpBitOffset == 0 )
            {
                _tmpByteIndex += 1;
                _tmpBitOffset = 8;
                _retP[ _tmpByteIndex ] = 0;
            }

            if( z != 0 )
            {
                _tmpHufBitOffset = z;
                continue;
            }
            _tmpHufByteIndex -= 1;
            _tmpHufBitOffset = 8;

        }while( x < pHuffmanCodeP[ i ].mBitS );

        i += 1;
    }

    return _tmpByteIndex;
}

uint8_t * fun_HuffmanCodeData( HuffmanResult_S * pHuffmanCodeP, uint32_t pHuffmanCodeLen, uint8_t * pDataP, uint32_t pDataLen, uint32_t * pRetLen )
{
    uint8_t * _retP;
    uint32_t _tmpLen, i;
    uint32_t _tmpCodeMapLen;

    _retP = NULL;
    _tmpLen = 1024;
    _retP = malloc( _tmpLen );

    i = fun_HuffmanCodeMapCreate( pHuffmanCodeP, pHuffmanCodeLen, _retP );
    _tmpCodeMapLen = i - 8;

    _retP[ 4 ] = _tmpCodeMapLen >> 24;
    _retP[ 5 ] = _tmpCodeMapLen >> 16;
    _retP[ 6 ] = _tmpCodeMapLen >> 8;
    _retP[ 7 ] = _tmpCodeMapLen & 0xff;

    return _retP;
}


HuffmanResult_S * fun_CreateHuffmanCode( uint8_t * pDestDataP, uint32_t pDestDataLen, int * pRetLen )
{
    uint32_t i, j, _tmpLen;
    uint32_t _tmpU32Value;
    uint8_t * _tmpP;
    HuffmanResult_S * _retP;
    uint32_t _tmpValueS[ 256 ];
    uint32_t _tmpCodeS[ 256 ];
    HuffmanResult_S * _tmpHuffmanResultArrayP;

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

        _tmpHuffmanResultArrayP = fun_GetHuffmanResultByHuffmanTree( _tmpHuffmanRootP, _tmpLen );

        if( _tmpHuffmanResultArrayP != NULL )
        {
            sub_ClearTree( _tmpHuffmanRootP );

            memcpy( _tmpValueS, _tmpCodeS, sizeof( uint32_t ) * 256 );

            *pRetLen = _tmpLen;

            _retP = ( HuffmanResult_S * )malloc( sizeof( HuffmanResult_S ) * _tmpLen );
            for( i = 0; i < _tmpLen; i++ )
            {
                _tmpU32Value = _tmpHuffmanResultArrayP[ i ].mDestValue;
                for( j = 0; j < 256; j++ )
                {
                    if( _tmpCodeS[ j ] == _tmpU32Value )
                    {
                        break;
                    }
                }

                if( j == 256 )
                {
                    free( _retP );
                    _retP = NULL;
                    break;
                }
                else
                {
                    _retP[ i ].mDestValue = j;
                    _retP[ i ].mBitS = _tmpHuffmanResultArrayP[ i ].mBitS;
                    memcpy( _retP[ i ].mHuffmanValue, _tmpHuffmanResultArrayP[ i ].mHuffmanValue, 16 );
                    _tmpCodeS[ j ] = 0;
                }
            }

            _tmpP = NULL;
            if( _retP != NULL )
            {
                _tmpP = fun_HuffmanCodeData( _retP, _tmpLen, pDestDataP, pDestDataLen, &j );
            }
        }

        free( _tmpHuffmanResultArrayP );
    }

    uint32_t _tmpTest = 0;
    if( _retP != 0 )
    {
        _tmpU32Value = 0;
        for( i = 0; i < 256; i++ )
        {
            for( j = 0; j < _tmpLen; j++ )
            {
                if( i == _retP[ j ].mDestValue )
                {
                    _tmpU32Value += _tmpValueS[ i ] * _retP[ j ].mBitS;
                    _tmpTest += _retP[ j ].mBitS;
                    break;
                }
            }
        }
    }

    return _retP;
}

