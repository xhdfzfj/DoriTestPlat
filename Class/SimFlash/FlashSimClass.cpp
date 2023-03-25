#include "FlashSimClass.h"

/**
 * @brief FlashSimClass::FlashSimClass
 */
FlashSimClass::FlashSimClass()
{
    mSimFlashSize = 16 * 1024 * 1024;

    mSimFlashP = new uint8_t [ mSimFlashSize ];
}

/**
 * @brief FlashSimClass::~FlashSimClass
 */
FlashSimClass::~FlashSimClass()
{
    delete [] mSimFlashP;
}
