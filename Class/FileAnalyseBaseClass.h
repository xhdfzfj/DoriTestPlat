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

protected:
    int fun_GetFileData( uint8_t * pSaveBufP, int pLen, int pStartOffset = -1 );
    void sub_ClearHandle() override;

protected:
    string mFilePath;
    bool mActiveFlag;
    int mCurrOffset;
    int mReadedLen;
    int mFileLen;
    std::ifstream mInFileStream;
};

#endif // FILEANALYSEBASECLASS_H
