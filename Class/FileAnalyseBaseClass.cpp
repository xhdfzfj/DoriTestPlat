#include <QtGlobal>
#include <memory.h>
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
    mReadedLen = 0;

    mMemoryDataSourceFlag = false;
    mMemoryDataP = nullptr;
    mMemoryDataLen = 0;
}


FileAnalyseBaseClass::FileAnalyseBaseClass()
{
    mFilePath = "";
    mCurrOffset = 0;
    mActiveFlag = false;
    mReadedLen = 0;

    mMemoryDataSourceFlag = false;
    mMemoryDataP = nullptr;
    mMemoryDataLen = 0;
}

/**
 * @brief FileAnalyseBaseClass::~FileAnalyseBaseClass
 */
FileAnalyseBaseClass::~FileAnalyseBaseClass()
{
    sub_ClearHandle();

    if( mMemoryDataP != nullptr )
    {
        delete [] mMemoryDataP;
    }
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
 * @brief FileAnalyseBaseClass::sub_SetDataSource
 * @param pDataP
 * @param pDataLen
 */
void FileAnalyseBaseClass::sub_SetDataSource( uint8_t * pDataP, uint32_t pDataLen )
{
    mMemoryDataSourceFlag = true;
    if( mMemoryDataP != nullptr )
    {
        delete [] mMemoryDataP;
    }

    mMemoryDataP = new uint8_t [ pDataLen ];
    mMemoryDataLen = pDataLen;
    memcpy( mMemoryDataP, pDataP, pDataLen );

    mCurrOffset = 0;
}

/**
 * @brief FileAnalyseBaseClass::fun_GetAllFileData
 * @return
 */
uint8_t * FileAnalyseBaseClass::fun_GetAllFileData( void )
{
    uint8_t * _retP;
    int _tmpOffset;

    _retP = nullptr;
    if( mActiveFlag )
    {
        _tmpOffset = mCurrOffset;
        _retP = new uint8_t [ mFileLen ];

        mInFileStream.seekg( 0, std::ios::beg );
        mInFileStream.read( ( char * )_retP, mFileLen );
        mInFileStream.seekg( _tmpOffset, std::ios::beg );
    }

    return _retP;
}

/**
 * @brief FileAnalyseBaseClass::fun_FileSeek
 * @param pStartOffset
 * @return
 */
bool FileAnalyseBaseClass::fun_FileSeek( int pStartOffset )
{
    bool _ret;

    _ret = false;
    if( mMemoryDataSourceFlag )
    {
        if( pStartOffset < mMemoryDataLen )
        {
            _ret = true;
            mCurrOffset = pStartOffset;
        }
    }
    else
    {

        if( !mActiveFlag )
        {
            std::string _tmpFilePath;

            if( mFilePath.length() < 3 )
            {
                return _ret;
            }

    #ifdef Q_OS_WIN
            _tmpFilePath = mFilePath.substr( 1, mFilePath.length() - 1 );
    #else
            _tmpFilePath = mFilePath;
    #endif
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

        if( mActiveFlag )
        {
            if( pStartOffset < mFileLen )
            {
                mInFileStream.seekg( pStartOffset, std::ios::beg );
                mCurrOffset = pStartOffset;
                _ret = true;
            }
        }
    }
    return _ret;
}

/**
 * @brief FileAnalyseBaseClass::sub_ClearCurrDataSource
 * @return
 */
void FileAnalyseBaseClass::sub_ClearCurrDataSource()
{
    if( mMemoryDataSourceFlag )
    {
        delete [] mMemoryDataP;
        mMemoryDataP = nullptr;
        mMemoryDataLen = 0;
    }
    else
    {
        if( mActiveFlag )
        {
            mInFileStream.close();
            mActiveFlag = false;
        }

        mFilePath = "";
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

    if( mMemoryDataSourceFlag )
    {
        _retValue = 0;
        if( mCurrOffset < mMemoryDataLen )
        {
            //_retValue = mCurrOffset + pLen;
            _retValue = mMemoryDataLen - mCurrOffset;
            if( _retValue > 0 )
            {
                if( _retValue > pLen )
                {
                    _retValue = pLen;
                }
            }
            else
            {
                _retValue = mMemoryDataLen - mCurrOffset;
            }

            memcpy( pSaveBufP, &mMemoryDataP[ mCurrOffset ], _retValue );
            mCurrOffset += _retValue;

            mReadedLen = _retValue;
        }
    }
    else
    {
        if( !mActiveFlag )
        {
            std::string _tmpFilePath;

    #ifdef Q_OS_WIN
            _tmpFilePath = mFilePath.substr( 1, mFilePath.length() - 1 );
    #else
            _tmpFilePath = mFilePath;
    #endif
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
            //_retValue = mCurrOffset + pLen;
            _retValue = mFileLen - mCurrOffset;
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
        else
        {
            _retValue = 0;
        }
        mInFileStream.read( ( char * )pSaveBufP, _retValue );

        mReadedLen = _retValue;
    }

    return _retValue;
}
