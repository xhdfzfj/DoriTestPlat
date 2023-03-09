#include "GuiDrawControl.h"

/**
 * @brief GuiDrawControl::GuiDrawControl
 * @param pParent
 */
GuiDrawControl::GuiDrawControl(QQuickItem * pParent) : QQuickPaintedItem( pParent )
{
    mFont = QFont( "SimSun", 16 );
    mMainImageP = nullptr;
}

/**
 * @brief GuiDrawControl::~GuiDrawControl
 */
GuiDrawControl::~GuiDrawControl()
{
    std::map< int, HexDataClass * >::iterator _itm;
    HexDataClass * _hexDataObjP;

    if( !mHexDataS.empty() )
    {
        for( _itm = mHexDataS.begin(); _itm != mHexDataS.end(); _itm++ )
        {
            _hexDataObjP = _itm->second;
            delete _hexDataObjP;
        }

        mHexDataS.clear();
    }

    if( mMainImageP != nullptr )
    {
        delete mMainImageP;
    }
}

/**
 * @brief GuiDrawControl::sub_MergeHexDataS
 *      合并HEX连续址址的数据
 */
void GuiDrawControl::sub_MergeHexDataS( void )
{
    bool _mergeFlag;
    HexDataClass * _hexDataObjP;
    HexDataClass * _prevHexDataObjP;
    uint8_t * _tmpP;
    int _tmpLen;

    _hexDataObjP = nullptr;
    _prevHexDataObjP = nullptr;
    if( !mHexDataS.empty() )
    {
        for( auto _tmpItm : mHexDataS )
        {
            _mergeFlag = false;
            _hexDataObjP = _tmpItm.second;
            if( _prevHexDataObjP != nullptr )
            {
                if( _prevHexDataObjP->GetHexDataEndAddress() == _hexDataObjP->GetStartOffset() )
                {
                    //合并两者
                    _mergeFlag = true;
                    _tmpP = _hexDataObjP->GetDataAndLen( _tmpLen );
                    _prevHexDataObjP->sub_AddData( _tmpP, _tmpLen );
                }
            }

            if( !_mergeFlag )
            {
                _prevHexDataObjP = _hexDataObjP;
            }
        }
    }
}

/**
 * @brief GuiDrawControl::sub_HexDataInput
 *      输入要显示的数据
 * @param pDataP
 * @param pDataLen
 * @param pStartOffset
 */
void GuiDrawControl::sub_HexDataInput( uint8_t * pDataP, int pDataLen, int pStartOffset )
{
    HexDataClass * _hexDataObjP;

    _hexDataObjP = new HexDataClass( pDataP, pDataLen, pStartOffset );
    if( mHexDataS.count( pStartOffset ) )
    {
        _hexDataObjP = mHexDataS.at( pStartOffset );
        delete _hexDataObjP;
    }

    mHexDataS[ pStartOffset ] = _hexDataObjP;

    sub_HexDataDraw();
}

/**
 * @brief GuiDrawControl::fun_CalcLineDisplayCount
 *      计算当前宽度下一行能显示的个数(宽度已减去前导部份)
 * @param pWidth
 *      总的行宽度
 * @param pHexDataWidth
 *      一个显示 FF 的宽度
 * @param pSpaceWidth
 *      显示 FF 与 FF 之间的间隔大小
 * @return
 *      -1 代表当前宽度过小无法进行显示
 */
int GuiDrawControl::fun_CalcLineDisplayCount( int pWidth, int pHexDataWidth, int pSpaceWidth )
{
    int _retValue;
    int _tmpValue;
    int i, j;

    _retValue = -1;
    i = 0;
    j = 0;

    i = 4;
    while( 1 )
    {
        j = i;

        _tmpValue = i * pHexDataWidth + ( i - 1 ) * pSpaceWidth;
        if( _tmpValue > pWidth )
        {
            break;
        }
        i *= 2;
    }

    _retValue = j;
    return _retValue;
}

/**
 * @brief GuiDrawControl::fun_CalcDisplayHeight
 *      计算出实际进行显示所要的高度
 * @return
 */
int GuiDrawControl::fun_CalcDisplayHeight( int pStrHeight, int _pLineByteS )
{
    int _retValue;
    int _lines;
    HexDataClass * _hexDataObjP;

    _retValue = 0;

    if( !mHexDataS.empty() )
    {
        for( auto _tmpItm : mHexDataS )
        {
            _hexDataObjP = _tmpItm.second;
            _lines = _hexDataObjP->GetDataLen();
            _lines += ( _pLineByteS - 1 );
            _lines /= _pLineByteS;

            _retValue += ( _lines * pStrHeight );
        }
    }

    return _retValue;
}

/**
 * @brief GuiDrawControl::sub_DrawHexDataToImage
 */
void GuiDrawControl::sub_DrawHexDataToImage( int pStrWidth, int pColonWidth, int pLineHeight, int pLineByteCount )
{
    int _imageWidth, _imageHeight;
    char _tmpTransferBuf[ 32 ];

    if( ( mMainImageP != nullptr ) && ( !mHexDataS.empty() ) )
    {
        uint32_t _tmpU32Value;
        int _currentX, _currentY;
        QPainter _tmpPainter( mMainImageP );
        HexDataClass * _hexDataObjP;
        QString _tmpQStr;
        int _tmpLen;

        _currentX = pColonWidth;
        _currentY = 0;
        _imageWidth = mMainImageP->width();
        _imageHeight = mMainImageP->height();

        for( auto _tmpItm : mHexDataS )
        {
            _hexDataObjP = _tmpItm.second;

            _tmpU32Value = _hexDataObjP->GetStartOffset();
            _tmpLen = _hexDataObjP->GetDataLen();

            sprintf( _tmpTransferBuf, "%08X", _tmpU32Value );
            _tmpQStr = QString::fromStdString( std::string( _tmpTransferBuf ) );
        }
    }
}

/**
 * @brief GuiDrawControl::sub_HexDataDraw
 */
void GuiDrawControl::sub_HexDataDraw( void )
{
    int _width, _height;
    int _ActualHeight, _strHeight;
    int _tmpWidth;
    int _stringWidth;
    int _spaceWidth;
    int _colonWidth;
    int _startOffset;

    HexDataClass * _hDataObjP;
    QFontMetrics _tmpFm( mFont );

    QString _tmpTestString = "FF";

    _stringWidth = _tmpFm.horizontalAdvance( _tmpTestString );

    _tmpTestString = "F";
    _spaceWidth = _tmpFm.horizontalAdvance( _tmpTestString );

    _strHeight = _tmpFm.height();

    _tmpTestString = ":";
    _colonWidth = _tmpFm.horizontalAdvance( _tmpTestString );

    _width = width();
    _height = height();

    int _lineBytes;

    _tmpWidth = _width - 2 * _colonWidth - 4 * _stringWidth;

    _lineBytes = fun_CalcLineDisplayCount( _tmpWidth, _stringWidth, _spaceWidth );
    if( _lineBytes < 0 )
    {
        return;
    }

    _startOffset = -1;

    sub_MergeHexDataS();

    _ActualHeight = fun_CalcDisplayHeight( _strHeight, _lineBytes );
    if( _ActualHeight < _height )
    {
        _ActualHeight = _height;
    }

    if( mMainImageP != nullptr )
    {
        delete mMainImageP;
    }

    mMainImageP = new QImage( _width, _ActualHeight, QImage::Format_ARGB32 );
    mMainImageP->fill( fillColor() );

    //mMainImageP->save( "test.jpg" );

    sub_DrawHexDataToImage( _stringWidth, _colonWidth, _ActualHeight, _lineBytes );
}

/**
 * @brief GuiDrawControl::paint
 * @param pPainter
 */
void GuiDrawControl::paint( QPainter * pPainter )
{
    //paint( pPainter );  //调用基类的重绘函数
    if( mMainImageP != nullptr )
    {
        pPainter->drawImage( 0, 0, *mMainImageP );
    }
}
