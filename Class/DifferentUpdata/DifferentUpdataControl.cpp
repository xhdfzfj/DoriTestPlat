
#include "../../Class/PrivateEventClass.h"
#include "../../Model/MainModelClass.h"
#include "DifferentUpdataControl.h"
#include "../../BsDiff/bsdiff.h"

DifferentUpdataControl::DifferentUpdataControl( QQuickItem * pParent ) : HexDataDisplayControl( pParent ), FileAnalyseBaseClass()
{
    mMainModelObjP = nullptr;
    mCurrDisplayStartY = -1;
    mCurrDisplayEndY = -1;

    mCurrStartDisplayLine = 0;
}

DifferentUpdataControl::~DifferentUpdataControl()
{
    qDebug() << "~DifferentUpdataControl";
}


void DifferentUpdataControl::sub_GetOtherFileDataAndLen( uint8_t * pFileDataP, int pFileDataLen )
{
    qDebug() << "Main Call Back";
}

/**
 * @brief DifferentUpdataControl::sub_CreateBsDiffFile
 */
void DifferentUpdataControl::sub_CreateBsDiffFile()
{
    int _tmpLen;
    PrivateEventClass * _tmpEventObjP;

    _tmpEventObjP = new PrivateEventClass( EventType_e::GetOldFileDataAndLen, DataType_e::DataType, Sender_e::DifferentUpdate,
                                           ( void * )this,  ( void * )( &this->sub_GetOtherFileDataAndLen ) );
    mMainModelObjP->sub_ChildObjectEventHandle( ( void * )_tmpEventObjP );

    //_tmpLen = fun_StartBsDiff( nullptr, 0, nullptr, 0, nullptr );
}

/**
 * @brief DifferentUpdataControl::sub_WheelEvent
 * @param pDirect
 *      0代表向下  1代表向上
 */
void DifferentUpdataControl::sub_WheelEvent( int pDirect, int pFlag )
{
    int _tmpLine;

    if( pFlag )
    {
        PrivateEventClass * _tmpEventObjP;

        _tmpEventObjP = new PrivateEventClass( EventType_e::WheelEvent, DataType_e::DataType, Sender_e::DifferentUpdate, ( void * )this, pDirect, 0 );
        mMainModelObjP->sub_ChildObjectEventHandle( ( void * )_tmpEventObjP );
    }
    _tmpLine = mCurrStartDisplayLine;
    if( pDirect == 0 )
    {
        //向下
        mCurrStartDisplayLine += 3;
        if( fun_FileSeek( mCurrStartDisplayLine * mLineByteCount ) )
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
