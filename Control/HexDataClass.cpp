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
