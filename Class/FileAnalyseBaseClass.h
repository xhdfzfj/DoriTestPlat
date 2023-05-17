#ifndef FILEANALYSEBASECLASS_H
#define FILEANALYSEBASECLASS_H

#include <string>
#include <cstddef>
#include <iostream>
#include <fstream>
#include "./SelfClearInterFaceClass.h"

using namespace std;

class FileAnalyseBaseClass : SelfClearInterFaceClass
{
public:
    FileAnalyseBaseClass();
    FileAnalyseBaseClass( string pFilePath );
    virtual ~FileAnalyseBaseClass();

    void sub_SetFilePath( std::string pFilePath ) { mFilePath = pFilePath; }
    int GetFileLen() { return mFileLen; }
    uint8_t * fun_GetAllFileData();
    void sub_SetDataSource( uint8_t * pDataP, uint32_t pDataLen );

protected:
    int fun_GetFileData( uint8_t * pSaveBufP, int pLen, int pStartOffset = -1 );
    bool fun_FileSeek( int pStartOffset );
    void sub_ClearHandle() override;

protected:
    string mFilePath;
    bool mActiveFlag;
    int mCurrOffset;
    int mReadedLen;
    int mFileLen;
    std::ifstream mInFileStream;

    bool mMemoryDataSourceFlag;
};

#endif // FILEANALYSEBASECLASS_H
