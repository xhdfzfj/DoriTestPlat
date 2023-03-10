#include <cstring>
#include "HexDataClass.h"

HexDataClass::HexDataClass( uint8_t * pDataP, int pDataLen, int pStartOffset )
{
    if( pDataP != nullptr )
    {
        mDataP = new uint8_t [ pDataLen ];
        memcpy( mDataP, pDataP, pDataLen );
        mDataLen = pDataLen;
        mStartOffset = pStartOffset;
    }
    else
    {
        mDataP = nullptr;
        mDataLen = 0;
    }
}

HexDataClass::~HexDataClass()
{
    if( mDataP != nullptr )
    {
        delete [] mDataP;
        mDataLen = 0;
    }
}

/**
 * @brief HexDataClass::sub_AddData
 * @param pDataP
 * @param pLen
 */
void HexDataClass::sub_AddData( uint8_t *pDataP, int pLen )
{
    uint8_t * _tmpP;

    _tmpP = new uint8_t [ pLen + mDataLen ];
    if( mDataP != nullptr )
    {
        memcpy( _tmpP, mDataP, mDataLen );
        delete [] mDataP;
    }

    memcpy( &_tmpP[ mDataLen ], pDataP, pLen );
    mDataLen += pLen;
    mDataP = _tmpP;
}
