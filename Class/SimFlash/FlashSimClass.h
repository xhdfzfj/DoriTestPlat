#ifndef FLASHSIMCLASS_H
#define FLASHSIMCLASS_H

#include <QObject>

/**
 * @brief The FlashSimClass class
 */
class FlashSimClass
{
public:
    FlashSimClass();
    virtual ~FlashSimClass();

    uint32_t GetSimFlashSize() { return mSimFlashSize; }

protected:
    int fun_GetData( int pStartOffset, int pGetCount, char * & pSaveP );

private:
    uint8_t * mSimFlashP;
    uint32_t mSimFlashSize;
};

#endif // FLASHSIMCLASS_H
