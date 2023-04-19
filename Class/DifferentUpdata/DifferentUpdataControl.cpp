
#include "../../Class/PrivateEventClass.h"
#include "../../Model/MainModelClass.h"
#include "DifferentUpdataControl.h"

DifferentUpdataControl::DifferentUpdataControl( QQuickItem * pParent ) : HexDataDisplayControl( pParent ), FileAnalyseBaseClass()
{
    mMainModelObjP = nullptr;
    mCurrDisplayStartY = -1;
    mCurrDisplayEndY = -1;
}

DifferentUpdataControl::~DifferentUpdataControl()
{

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
