#include "FileAnalyseBaseClass.h"

/**
 * @brief FileAnalyseBaseClass::FileAnalyseBaseClass
 * @param pFilePath
 */
FileAnalyseBaseClass::FileAnalyseBaseClass( string pFilePath )
{
    mFilePath = pFilePath;
    mCurrOffset = 0;
    mActiveFlag = false;
}

/**
 * @brief FileAnalyseBaseClass::~FileAnalyseBaseClass
 */
FileAnalyseBaseClass::~FileAnalyseBaseClass()
{
    sub_ClearHandle();
}

/**
 * @brief FileAnalyseBaseClass::sub_ClearHandle
 *      重载基类的清理函数
 */
void FileAnalyseBaseClass::sub_ClearHandle()
{
    if( mActiveFlag )
    {
        mInFileStream.close();
    }
}

/**
 * @brief FileAnalyseBaseClass::fun_GetFileData
 * @param pLen
 * @param pStartOffset
 * @return
 */
int FileAnalyseBaseClass::fun_GetFileData( uint8_t * pSaveBufP, int pLen, int pStartOffset )
{
    int _retValue;

    if( !mActiveFlag )
    {
        std::string _tmpFilePath;

        _tmpFilePath = mFilePath.substr( 1, mFilePath.length() - 1 );

        mInFileStream.open( _tmpFilePath.c_str(), std::ios::binary );
        if( mInFileStream )
        {
            mActiveFlag = true;
            mInFileStream.seekg( 0, std::ios::end );
            mFileLen = ( int )mInFileStream.tellg();
            mInFileStream.seekg( 0, std::ios::beg );

            mCurrOffset = 0;
        }
    }
    if( pStartOffset != -1 )
    {
       //从当前位置获取文件的数据
    }

    if( mCurrOffset < mFileLen )
    {
        _retValue = mCurrOffset + pLen;
        _retValue = mFileLen - _retValue;
        if( _retValue > 0 )
        {
            if( _retValue > pLen )
            {
                _retValue = pLen;
            }
        }
        else
        {
            _retValue = mFileLen - mCurrOffset;
        }

        mCurrOffset += _retValue;
    }
    mInFileStream.read( ( char * )pSaveBufP, pLen );

    return _retValue;
}
