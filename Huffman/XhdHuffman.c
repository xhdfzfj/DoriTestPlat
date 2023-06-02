#include "memory.h"
#include "./XhdHuffman.h"
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


uint8_t * fun_CreateHuffmanCode( uint8_t * pDestDataP, uint32_t pDestDataLen, int * pRetLen )
{
    uint32_t i, _tmpLen;
    uint8_t * _retP;
    uint32_t * _tmpValueSP;
    uint32_t _tmpCodeS[ 256 ];

    _retP = NULL;
    _tmpValueSP = NULL;
    memset( ( void * )_tmpCodeS, 0, sizeof( uint32_t ) * 256 );

    _tmpLen = pDestDataLen;

    for( i = 0; i < _tmpLen; i++ )
    {
        _tmpCodeS[ pDestDataP[ i ] ] += 1;
    }

    sub_MergSort( _tmpCodeS, 256 );
    for( i = 0; i < 256; i++ )
    {
        if( _tmpCodeS[ i ] != 0 )
        {
            break;
        }
    }

    if( i != 256 )
    {
        //不为全空的情况
        _tmpLen = 256 - i;
        if( _tmpLen != 256 )
        {
            _tmpValueSP = ( uint32_t * )malloc( _tmpLen * sizeof( uint32_t ) );
            memcpy( _tmpValueSP, &_tmpCodeS[ i ], _tmpLen * sizeof( uint32_t ) );

            aaaa
        }
    }

    if( _tmpValueSP != NULL )
    {
        free( _tmpValueSP );
    }

    return _retP;
}
