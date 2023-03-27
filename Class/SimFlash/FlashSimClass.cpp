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
