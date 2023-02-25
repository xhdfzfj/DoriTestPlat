#ifndef HEXDATACLASS_H
#define HEXDATACLASS_H

#include <string>

class HexDataClass
{
public:
    HexDataClass( uint8_t * pDataP, int pDataLen, int pStartOffset );
    virtual ~HexDataClass();

private:
    uint8_t * mDataP;
    int mDataLen;
    int mStartOffset;
};

#endif // HEXDATACLASS_H
