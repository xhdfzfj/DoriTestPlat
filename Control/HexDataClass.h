#ifndef HEXDATACLASS_H
#define HEXDATACLASS_H

#include <string>

class HexDataClass
{
public:
    HexDataClass( uint8_t * pDataP, int pDataLen, int pStartOffset );
    virtual ~HexDataClass();

    uint32_t GetHexDataEndAddress() { return ( uint32_t )( mStartOffset + mDataLen ); }
    uint32_t GetStartOffset() { return ( uint32_t )mStartOffset; }
    uint8_t * GetDataAndLen( int & pRetLen ) { pRetLen = mDataLen; return mDataP; }
    void sub_AddData( uint8_t * pDataP, int pLen );
    int GetDataLen() { return mDataLen; }
private:
    uint8_t * mDataP;
    int mDataLen;
    int mStartOffset;
};

#endif // HEXDATACLASS_H
