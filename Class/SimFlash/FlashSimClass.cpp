#include <string>
#include "FlashSimClass.h"

/**
 * @brief FlashSimClass::FlashSimClass
 */
FlashSimClass::FlashSimClass()
{
    mSimFlashSize = 16 * 1024 * 1024;

    mSimFlashP = new uint8_t [ mSimFlashSize ];

    for( int i = 0; i < mSimFlashSize; i++ )
    {
        mSimFlashP[ i ] = i;
    }
    //memset( mSimFlashP, 0xff, mSimFlashSize );
}

/**
 * @brief FlashSimClass::~FlashSimClass
 */
FlashSimClass::~FlashSimClass()
{
    delete [] mSimFlashP;
}

/**
 * @brief FlashSimClass::fun_GetData
 * @param pStartOffset
 * @param pGetCount
 * @param pSaveP
 * @return
 */
int FlashSimClass::fun_GetData( int pStartOffset, int pGetCount, char * & pSaveP )
{
    int _retValue;
    int _tmpValue;

    _retValue = 0;

    _tmpValue = mSimFlashSize - ( pStartOffset + pGetCount );
    if( _tmpValue > 0 )
    {
        if( _tmpValue >= pGetCount )
        {
            _tmpValue = pGetCount;
        }
    }
    else
    {
        if( pStartOffset <= ( int )( mSimFlashSize - 1 ) )
        {
            _tmpValue = mSimFlashSize - pStartOffset;
        }
    }

    if( _tmpValue > 0 )
    {
        memcpy( pSaveP, &mSimFlashP[ pStartOffset ], _tmpValue );
        _retValue = _tmpValue;
    }

    return _retValue;
}
