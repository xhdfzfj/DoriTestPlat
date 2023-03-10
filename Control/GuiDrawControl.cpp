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
    std::list< int > mDelKeyS;
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
                    mDelKeyS.push_back( _tmpItm.first );
                }
            }

            if( !_mergeFlag )
            {
                _prevHexDataObjP = _hexDataObjP;
            }
        }

        if( !mDelKeyS.empty() )
        {
            for( auto _key : mDelKeyS )
            {
                mHexDataS.erase( _key );
            }

            mDelKeyS.clear();
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

    qDebug() << "mHexDataS count " << mHexDataS.size();

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
        int i;
        QPen _tmpPen;

        _currentX = pColonWidth;
        _currentY = 0;
        _imageWidth = mMainImageP->width();
        _imageHeight = mMainImageP->height();

        _tmpPainter.setFont( mFont );
        _tmpPainter.setRenderHint( QPainter::Antialiasing );
        _tmpPen.setColor( Qt::black );

        for( auto _tmpItm : mHexDataS )
        {
            _hexDataObjP = _tmpItm.second;

            _tmpU32Value = _hexDataObjP->GetStartOffset();
            _tmpLen = _hexDataObjP->GetDataLen();

            i = 0;
            while( i < _tmpLen )
            {
                _currentX = 0;
                _currentY += pLineHeight;
                sprintf( _tmpTransferBuf, "%08X", _tmpU32Value );
                _tmpQStr = QString::fromStdString( std::string( _tmpTransferBuf ) );

                _tmpPainter.drawText( _currentX, _currentY, _tmpQStr );
                _currentX += ( pStrWidth * 4 );
                _tmpQStr = ":";
                _tmpPainter.drawText( _currentX, _currentY, _tmpQStr );
                _currentX += pColonWidth;

                _tmpQStr = fun_GetDataToHexDisplayStr( _hexDataObjP->GetDataBuf(), i, pLineByteCount );
                _tmpPainter.drawText( _currentX, _currentY, _tmpQStr );

                _tmpU32Value += pLineByteCount;
                i += pLineByteCount;
            }

            if( i > _tmpLen )
            {
                //是有尾数没有显示出来
                _currentX = 0;
                _currentY += pLineHeight;

                i -= pLineByteCount;
                _tmpLen = _tmpLen - i;

                _tmpU32Value -= pLineByteCount;

                sprintf( _tmpTransferBuf, "%08X", _tmpU32Value );
                _tmpQStr = QString::fromStdString( std::string( _tmpTransferBuf ) );

                _tmpPainter.drawText( _currentX, _currentY, _tmpQStr );
                _currentX += ( pStrWidth * 4 );
                _tmpQStr = ":";
                _tmpPainter.drawText( _currentX, _currentY, _tmpQStr );
                _currentX += pColonWidth;

                _tmpQStr = fun_GetDataToHexDisplayStr( _hexDataObjP->GetDataBuf(), i, _tmpLen );
                _tmpPainter.drawText( _currentX, _currentY, _tmpQStr );
            }
        }
    }
}

/**
 * @brief GuiDrawControl::fun_GetDataToHexDisplayStr
 * @param pDataP
 *      止标数据缓冲区
 * @param pOffset
 *      开始的偏移
 * @param pLen
 *      要获取的长度
 * @return
 */
QString GuiDrawControl::fun_GetDataToHexDisplayStr( uint8_t * pDataP, int pOffset, int pLen )
{
    QString _retStr;
    char * _tmpCharBufP;

    _tmpCharBufP = new char [ pLen * 2 + pLen ];

    int i;
    int j;

    j = 0;
    for( i = 0; i < pLen; i++ )
    {
        if( i != 0 )
        {
            _tmpCharBufP[ j ] = ' ';
            j += 1;
        }
        sprintf( &_tmpCharBufP[ j ], "%02X", pDataP[ i + pOffset ] );
        j += 2;
    }

    _retStr = QString::fromStdString( std::string( _tmpCharBufP ) );
    delete [] _tmpCharBufP;
    return _retStr;
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
    _ActualHeight += 3; //底部的间隔
    if( _ActualHeight < _height )
    {
        _ActualHeight = _height;
    }

    if( mMainImageP != nullptr )
    {
        delete mMainImageP;
    }

    mMainImageP = new QImage( _width, _ActualHeight, QImage::Format_ARGB32 );
    mMainImageP->fill( Qt::white );

    sub_DrawHexDataToImage( _stringWidth, _colonWidth, _strHeight, _lineBytes );

    //mMainImageP->save( "test.jpg" );
    //update();
}

/**
 * @brief GuiDrawControl::sub_TestFunction
 */
void GuiDrawControl::sub_TestFunction()
{
   if( mMainImageP != nullptr )
   {
       mMainImageP->save( "test.jpg" );
   }
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
        pPainter->setRenderHint( QPainter::Antialiasing );
        pPainter->drawImage( 0, 0, *mMainImageP );
    }
}
