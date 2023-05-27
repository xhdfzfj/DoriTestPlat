#include <QFile>
#include <QDataStream>
#include "../../Class/PrivateEventClass.h"
#include "../../Model/MainModelClass.h"
#include "DifferentUpdataControl.h"
#include "../../BsDiff/BsDiffWithOutZip.h"
#include "../../BsDiff/BsDiffRestore.h"

DifferentUpdataControl::DifferentUpdataControl( QQuickItem * pParent ) : HexDataDisplayControl( mContentMenuP, pParent ), FileAnalyseBaseClass()
{
    mMainModelObjP = nullptr;
    mCurrDisplayStartY = -1;
    mCurrDisplayEndY = -1;

    mCurrStartDisplayLine = 0;

    mOldDataContentP = nullptr;
    mOldDataContentLen = 0;

    mNewDataContentP = nullptr;
    mNewDataContentLen = 0;

    mContentMenuP = nullptr;
    mThreadWorkType = DifferenceThreadType::NoThreadWorkType;
    mWorkThreadP = nullptr;

    //setAcceptHoverEvents( true );
}

DifferentUpdataControl::~DifferentUpdataControl()
{
    qDebug() << "~DifferentUpdataControl";
    if( mOldDataContentP != nullptr )
    {
        delete [] mOldDataContentP;
    }

    if( mNewDataContentP != nullptr )
    {
        delete [] mNewDataContentP;
    }

    if( mContentMenuP != nullptr )
    {
        delete mContentMenuP;
    }
}

/**
 * @brief DifferentUpdataControl::sub_GetOldFileDataAndLenCB
 *      静态函数
 * @param pFileDataP
 * @param pFileDataLen
 * @param pDestObjP
 */
void DifferentUpdataControl::sub_GetOldFileDataAndLenCB( uint8_t * pFileDataP, int pFileDataLen, void * pDestObjP )
{
    qDebug() << "Main Call Back";

    DifferentUpdataControl * _tmpObjP;

    _tmpObjP = ( DifferentUpdataControl * )pDestObjP;

    _tmpObjP->sub_GetOtherFileDataAndLen( pFileDataP, pFileDataLen, 0 );

}

/**
 * @brief DifferentUpdataControl::sub_GetOldFileDataAndLenCB
 *      静态函数
 * @param pFileDataP
 * @param pFileDataLen
 * @param pDestObjP
 */
void DifferentUpdataControl::sub_GetNewFileDataAndLenCB( uint8_t * pFileDataP, int pFileDataLen, void * pDestObjP )
{
    qDebug() << "Main Call Back";

    DifferentUpdataControl * _tmpObjP;

    _tmpObjP = ( DifferentUpdataControl * )pDestObjP;

    _tmpObjP->sub_GetOtherFileDataAndLen( pFileDataP, pFileDataLen, 1 );

}

/**
 * @brief DifferentUpdataControl::sub_GetOtherFileDataAndLen
 * @param pDataP
 * @param pLen
 * @param pType
 */
void DifferentUpdataControl::sub_GetOtherFileDataAndLen( uint8_t * pDataP, int pLen, int pType )
{
    int _tmpLen;
    uint8_t * _resultP;

    if( pType == 0 )
    {
        if( mOldDataContentP != nullptr )
        {
            delete [] mOldDataContentP;
            mOldDataContentP = nullptr;
        }

        mOldDataContentP = new uint8_t [ pLen ];
        memcpy( mOldDataContentP, pDataP, pLen );
        mOldDataContentLen = pLen;
    }
    else
    {
        if( mNewDataContentP != nullptr )
        {
            delete [] mNewDataContentP;
            mNewDataContentP = nullptr;
        }

        mNewDataContentP = new uint8_t [ pLen ];
        memcpy( mNewDataContentP, pDataP, pLen );
        mNewDataContentLen = pLen;
    }

    if( ( mOldDataContentP != nullptr ) && ( mNewDataContentP != nullptr ) )
    {
        _tmpLen = fun_StartBsDiff( mOldDataContentP, mOldDataContentLen, mNewDataContentP, mNewDataContentLen, &_resultP );
        if( _tmpLen > 0 )
        {
            sub_SetDataSource( _resultP, _tmpLen );
            sub_FreeMallocAddr( _resultP );

            sub_ReadyCreateGui();
            ClrMainImage();
            ResetCurrXY();
            sub_DataToImage();
        }
    }
}

/**
 * @brief DifferentUpdataControl::sub_CreateBsDiffFile
 */
void DifferentUpdataControl::sub_CreateBsDiffFile()
{

    PrivateEventClass * _tmpEventObjP;


    _tmpEventObjP = new PrivateEventClass( EventType_e::GetOldFileDataAndLen, DataType_e::DataType, Sender_e::DifferentUpdate,
                                          ( void * )this,  ( void * )( DifferentUpdataControl::sub_GetOldFileDataAndLenCB ) );
    mMainModelObjP->sub_ChildObjectEventHandle( ( void * )_tmpEventObjP );

    _tmpEventObjP = new PrivateEventClass( EventType_e::GetNewFileDataAndLen, DataType_e::DataType, Sender_e::DifferentUpdate,
                                          ( void * )this,  ( void * )( DifferentUpdataControl::sub_GetNewFileDataAndLenCB ) );
    mMainModelObjP->sub_ChildObjectEventHandle( ( void * )_tmpEventObjP );

    //_tmpLen = fun_StartBsDiff( nullptr, 0, nullptr, 0, nullptr );
}

/**
 * @brief DifferentUpdataControl::sub_BsRestoreProcessDisplayClick
 *      恢复时的过程显示
 */
void DifferentUpdataControl::sub_BsRestoreProcessDisplayClick()
{
    BsRestoreState_e _tmpState;
    uint8_t * _tmpBufP;
    int _tmpLen;
    PrivateEventClass * _tmpEventObjP;

    if( mThreadWorkType == DifferenceRestoreType )
    {
        _tmpState = fun_GetBsDiffRestoreState();
        if( _tmpState == BufReadyOkState )
        {
            if( mDisplayCount == 0 )
            {
                _tmpBufP = fun_GetBsDiffRestoreBuf( mDisplayCount, &_tmpLen );

                if( _tmpBufP != nullptr )
                {

                    _tmpEventObjP = new PrivateEventClass( EventType_e::SetBsDiffNewDataSource, DataType_e::DataType, Sender_e::DifferentUpdate,
                                                           ( void * )_tmpBufP, _tmpLen, 0 );
                    _tmpEventObjP->SetFreeState( FreeParamType_e::NoFreeType );

                    mMainModelObjP->sub_ChildObjectEventHandle( ( void * )_tmpEventObjP );
                }
                mDisplayCount += 1;
                if( mDisplayCount >= 3 )
                {
                    mDisplayCount = 0;
                }
            }
        }
    }
}

/**
 * @brief DifferentUpdataControl::sub_WheelEvent
 * @param pDirect
 *      0代表向下  1代表向上
 */
void DifferentUpdataControl::sub_WheelEvent( int pDirect, int pFlag )
{
    int _tmpLine;
    int _tmpValue;

    if( pFlag && ( mDifferentDisplayType != 2 ) )
    {
        PrivateEventClass * _tmpEventObjP;

        _tmpEventObjP = new PrivateEventClass( EventType_e::WheelEvent, DataType_e::DataType, Sender_e::DifferentUpdate, ( void * )this, pDirect, 0 );
        _tmpEventObjP->SetFreeState( FreeParamType_e::NoFreeType );
        mMainModelObjP->sub_ChildObjectEventHandle( ( void * )_tmpEventObjP );

    }
    _tmpLine = mCurrStartDisplayLine;
    if( pDirect == 0 )
    {
        //向下
        mCurrStartDisplayLine += 3;
        _tmpValue = mCurrStartDisplayLine * mLineByteCount;
        if( fun_FileSeek( _tmpValue ) )
        {
            ClrMainImage();
            ResetCurrXY();
            sub_DataToImage();
        }
        else
        {
            mCurrStartDisplayLine = _tmpLine;
        }
    }
    else
    {
        //向上
        mCurrStartDisplayLine -= 3;
        if( mCurrStartDisplayLine < 0 )
        {
            mCurrStartDisplayLine = 0;
        }
        if( fun_FileSeek( mCurrStartDisplayLine * mLineByteCount ) )
        {
            ClrMainImage();
            ResetCurrXY();
            sub_DataToImage();
        }
    }
}

/**
 * @brief DifferentUpdataControl::sub_MouseRightButtonClick
 * @param pX
 * @param pY
 */
void DifferentUpdataControl::sub_MouseRightButtonClick( qreal pX, qreal pY )
{
    QPoint _tmpPoint( pX, pY );

    if( mContentMenuP != nullptr )
    {
        mMenuDisplayFlag = true;
        mContentMenuP->sub_SetDisplayPoint( _tmpPoint );
        emit sub_SignalReDraw();
    }
}

/**
 * @brief DifferentUpdataControl::sub_MouseLeftButtonClick
 * @param pX
 * @param pY
 */
void DifferentUpdataControl::sub_MouseLeftButtonClick( qreal pX, qreal pY )
{
    QPoint _tmpPoint( pX, pY );
    QRect _tmpRect;
    int _tmpCount;
    int _tmpIndex;
    QRect * mContentItemRectP;

    if( mContentMenuP != nullptr )
    {
        if( mMenuDisplayFlag )
        {
            mContentItemRectP = mContentMenuP->fun_GetDisplayMenuItemRect( _tmpCount );
            _tmpIndex = mContentMenuP->GetSelectIndex();
            if( _tmpIndex != -1 )
            {
                _tmpRect = mContentItemRectP[ _tmpIndex ];
                if( _tmpRect.contains( _tmpPoint ) )
                {
                    //菜单处理
                    if( _tmpIndex == 0 )
                    {
                        sub_SaveBsDiffFile();
                    }
                    else if( _tmpIndex == 1 )
                    {
                        sub_StartRestoreBsDiff();
                    }
                }
            }
            mContentMenuP->sub_SetDisplayPoint( QPoint( -1, -1 ) );
            mMenuDisplayFlag = false;
            emit sub_SignalReDraw();
        }
    }
}

/**
 * @brief DifferentUpdataControl::sub_RestoreBsDiffHandle
 */
void DifferentUpdataControl::sub_RestoreBsDiffHandle( void )
{
    int _ret;

    _ret = fun_BsDiffRestore( mOldDataContentP, mOldDataContentLen, mMemoryDataP, mMemoryDataLen, 0 );

    if( _ret == 2 )
    {
        //可以继续进行
        _ret = fun_BsDiffRestore( mOldDataContentP, mOldDataContentLen, mMemoryDataP, mMemoryDataLen, 1 );

    }
}

/**
 * @brief DifferentUpdataControl::sub_StartRestoreBsDiff
 *      启动BSDIFF文件的恢复
 */
void DifferentUpdataControl::sub_StartRestoreBsDiff( void )
{
    mThreadWorkType = DifferenceThreadType::DifferenceRestoreType;
    mDisplayCount = 0;
    if( mWorkThreadP != nullptr )
    {
        mWorkThreadP->detach(); //分离线程
        delete mWorkThreadP;    //分离出来的线程，可以强行终止
        mWorkThreadP = nullptr;
    }

    mWorkThreadP = new std::thread( &DifferentUpdataControl::sub_RestoreBsDiffHandle, this );

    PrivateEventClass * _tmpEventObjP;
    _tmpEventObjP = new PrivateEventClass( EventType_e::logInfoType, DataType_e::StringType, "启动BSDIFF恢复线程" );    //不用清理,接口处清理
    _tmpEventObjP->SetLogLevel( xhdLogEventClass::logInfo );
    mMainModelObjP->sub_ChildObjectEventHandle( ( void * )_tmpEventObjP );

    _tmpEventObjP = new PrivateEventClass( EventType_e::ClearBsDiffNewFileDisplay, DataType_e::DataType, Sender_e::DifferentUpdate, nullptr, 0, 0 );
    _tmpEventObjP->SetFreeState( FreeParamType_e::NoFreeType );
    mMainModelObjP->sub_ChildObjectEventHandle( ( void * )_tmpEventObjP );

}

/**
 * @brief DifferentUpdataControl::sub_SetMainModelObj
 * @param pObjectP
 */
void DifferentUpdataControl::sub_SetMainModelObj( QObject * pObjectP )
{
    mMainModelObjP = ( MainModelClass * )pObjectP;

    PrivateEventClass * _tmpEventObjP;

    _tmpEventObjP = new PrivateEventClass( EventType_e::RegisterObject, DataType_e::DataType, Sender_e::DifferentUpdate, ( void * )this );
    mMainModelObjP->sub_ChildObjectEventHandle( ( void * )_tmpEventObjP );
}

/**
 * @brief DifferentUpdataControl::sub_DifferentType
 * @param pState
 */
void DifferentUpdataControl::sub_DifferentType( int pState )
{
    mDifferentDisplayType = pState;
    if( mDifferentDisplayType == 2 )
    {
        if( mContentMenuP != nullptr )
        {
            delete mContentMenuP;
        }
        mContentMenuP = new PrivateMenuClass();
        mContentMenuP->SetMenuFont( GetHexDisplayFont() );

        mContentMenuP->sub_AddMenuItem( "保存文件" );
        mContentMenuP->sub_AddMenuItem( "恢复文件" );
//        mContentMenuP->sub_AddMenuItem( "测试菜单1" );
//        mContentMenuP->sub_AddMenuItem( "测试菜单2" );
//        mContentMenuP->sub_AddMenuItem( "测试菜单3" );
//        mContentMenuP->sub_AddMenuItem( "测试菜单4" );
    }
}

/**
 * @brief DifferentUpdataControl::sub_DifferentFile
 * @param pFilePath
 */
void DifferentUpdataControl::sub_DifferentFile( QUrl pFilePath )
{
    if( pFilePath.isLocalFile() )
    {
        std::string _tmpStr;

        _tmpStr = pFilePath.path().toStdString();

        sub_SetFilePath( _tmpStr );

        PrivateEventClass * _tmpEventObjP;

        if( mDifferentDisplayType == 0 )
        {
            _tmpStr = "旧文件:" + _tmpStr;
        }
        else if( mDifferentDisplayType == 1 )
        {
            _tmpStr = "新文件:" + _tmpStr;
        }

        _tmpEventObjP = new PrivateEventClass( EventType_e::logInfoType, DataType_e::StringType, _tmpStr );
        mMainModelObjP->sub_ChildObjectEventHandle( ( void * )_tmpEventObjP );

        if( ( mDifferentDisplayType == 0 ) || ( mDifferentDisplayType == 1 ) )
        {
            sub_ReadyCreateGui();
            if( mLineByteCount == 0 )
            {
                _tmpStr = "显示区域太小，无法显示";
                _tmpEventObjP = new PrivateEventClass( EventType_e::logInfoType, DataType_e::StringType, _tmpStr );
                mMainModelObjP->sub_ChildObjectEventHandle( ( void * )_tmpEventObjP );
            }
            else
            {
                sub_DataToImage();
            }
        }
    }
}

/**
 * @brief DifferentUpdataControl::sub_SaveBsDiffFile
 */
void DifferentUpdataControl::sub_SaveBsDiffFile()
{
    uint8_t * _tmpP;
    int _tmpLen;
    QString _tmpFilePath;
    QFile _tmpFile( "BsDiff.bin" );

    if( mDifferentDisplayType == 2 )
    {
        if( mMemoryDataSourceFlag )
        {
            _tmpFile.open( QIODevice::WriteOnly );
            QDataStream _ds( &_tmpFile );

            _tmpP = mMemoryDataP;
            if( _tmpP != nullptr )
            {
                _tmpLen = mMemoryDataLen;
                for( int i = 0; i < _tmpLen; i++ )
                {
                    _ds << ( uint8_t )_tmpP[ i ];
                }
            }

            _tmpFile.close();
        }
    }
}

/**
 * @brief DifferentUpdataControl::sub_ReadyCreateGui
 *      准备进行GUI的显示
 */
void DifferentUpdataControl::sub_ReadyCreateGui( void )
{
    int _tmpWidth;
    int _lineByteCount;

    _lineByteCount = 0;
    _tmpWidth = width() - mAddressStringWidth - mColonFontWidth - mSpaceValue - mSpaceValue ;

    int i, j;
    int _tmpValue;

    i = 8;

    _tmpValue = i * mDataFontWidth + ( i - 1 ) * mSingleAscWidth;

    if( _tmpValue > _tmpWidth )
    {
        _lineByteCount = 0;
    }
    else
    {
        while( _tmpValue <= _tmpWidth )
        {
            j = i;
            i *= 2;
            _tmpValue = i * mDataFontWidth + ( i - 1 ) * mSingleAscWidth;
        }

        _lineByteCount = j;
    }

    mLineByteCount = _lineByteCount;
}

/**
 * @brief DifferentUpdataControl::sub_DataToImage
 */
void DifferentUpdataControl::sub_DataToImage()
{
    uint8_t * _tmpLineBufP;
    int _retValue;
    int _y, _tmpOffset;
    bool _flag;

    _y = mFontHeight;
    _tmpOffset = mCurrOffset;
    _tmpLineBufP = new uint8_t [ mLineByteCount ];

    _retValue = fun_GetFileData( _tmpLineBufP, mLineByteCount );

    if( _retValue != 0 )
    {
        setXToInit();
        _flag = fun_LineHexDataToImage( _tmpLineBufP, mLineByteCount, _tmpOffset );
        _tmpOffset += _retValue;

        while( !_flag )
        {
            _retValue = fun_GetFileData( _tmpLineBufP, mLineByteCount );
            if( _retValue == 0 )
            {
                break;
            }
            setXToInit();
            _flag = fun_LineHexDataToImage( _tmpLineBufP, mLineByteCount, _tmpOffset );
            _tmpOffset += _retValue;
        }

        saveMainImageToJpg( "diffrent.jpg" );

        emit sub_SignalReDraw();
    }

}

/**
 * @brief DifferentUpdataControl::sub_ClearCurrentDisplayData
 */
void DifferentUpdataControl::sub_ClearCurrentDisplayData( void )
{
    ClrMainImage();

    sub_ClearCurrDataSource();

    emit sub_SignalReDraw();
}


