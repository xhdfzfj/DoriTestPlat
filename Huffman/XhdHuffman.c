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
    uint32_t _tmpCodeS[ 256 ];

    _retP = NULL;
    memset( ( void * )_tmpCodeS, 0, sizeof( uint32_t ) * 256 );

    _tmpLen = pDestDataLen;

    for( i = 0; i < _tmpLen; i++ )
    {
        _tmpCodeS[ pDestDataP[ i ] ] += 1;
    }


    uint32_t _testBuf[ 12 ];

    _testBuf[ 0 ] = 6;
    _testBuf[ 1 ] = 7;
    _testBuf[ 2 ] = 5;
    _testBuf[ 3 ] = 7;
    _testBuf[ 4 ] = 9;
    _testBuf[ 5 ] = 6;
    _testBuf[ 6 ] = 4;
    _testBuf[ 7 ] = 10;
    _testBuf[ 8 ] = 3;
    _testBuf[ 9 ] = 2;
    _testBuf[ 10 ] = 8;
    _testBuf[ 11 ] = 91;

    sub_MergSort( _testBuf, 12 );

    return _retP;
}
