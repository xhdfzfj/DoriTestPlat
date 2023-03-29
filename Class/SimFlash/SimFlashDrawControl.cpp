﻿#include <QQmlProperty>
#include "SimFlashDrawControl.h"

SimFlashDrawControl::SimFlashDrawControl(QQuickItem * pParent) : QQuickPaintedItem( pParent )
{
    mFont = QFont( "SimSun", 16 );
    //mFont.setStyleStrategy( QFont::NoAntialias );
    mFontSize = 16;

    mCurrDisplayIndex = 0;
    mCurrDisplayEndIndex = 0;
    mLineByteS = 0;

    mAddressTableFontColor = Qt::white;
    mAddressTableBackColor = Qt::black;
    mBackColor = Qt::white;
    mMainImageP = nullptr;
    mScrollBarP = nullptr;

    mStopScrollChanageFlag = false;

    mDragFlag = false;
    mDragSelectFlag = false;
    mSaveDragRectImageP = nullptr;
    mPrevDragWidth = 0;

    connect( this, SIGNAL( sub_SignalReDraw() ), this, SLOT( sub_SlotReDraw() ), Qt::QueuedConnection );

}


SimFlashDrawControl::~SimFlashDrawControl()
{
    if( mMainImageP != nullptr )
    {
        delete mMainImageP;
    }

    if( mSaveDragRectImageP != nullptr )
    {
        delete mSaveDragRectImageP;
    }
}

/**
 * @brief SimFlashDrawControl::sub_QmlLoadered
 *      Qml装载完成
 */
void SimFlashDrawControl::sub_QmlLoadered( QObject * pObjectP )
{
    qDebug() << "simflash width " << width();
    mScrollBarP = pObjectP;
    mMainImageP = new QImage( width(), height(), QImage::Format_ARGB32 );
    mMainImageP->fill( mBackColor );
    sub_DataToImage();
}

/**
 * @brief SimFlashDrawControl::sub_ChanageScrollValue
 */
void SimFlashDrawControl::sub_ChanageScrollValue()
{
    float _tmpValue;

    _tmpValue = ( float )mCurrDisplayIndex / ( float )GetSimFlashSize();
    mStopScrollChanageFlag = true;
    QQmlProperty::write( mScrollBarP, "position", _tmpValue );
}

/**
 * @brief SimFlashDrawControl::sub_WheelEvent
 * @param pDirect
 *      0 代表向下
 *      1 代表向上
 */
void SimFlashDrawControl::sub_WheelEvent( int pDirect )
{
    int _tmpValue;
    int i;

    if( mSaveDragRectImageP != nullptr )
    {
        delete mSaveDragRectImageP;
        mSaveDragRectImageP = nullptr;
        mDragFlag = false;
        mDragSelectFlag = false;
    }

    if( pDirect == 0 )
    {
        //向下
        i = 3;
        _tmpValue = mCurrDisplayIndex;
        if( mCurrDisplayEndIndex < GetSimFlashSize() )
        {
            _tmpValue += ( i * mLineByteS );
            while( _tmpValue >= GetSimFlashSize() )
            {
                i -= 1;
                if( i == 0 )
                {
                    _tmpValue = 0;
                    break;
                }
                _tmpValue = mCurrDisplayIndex;
                _tmpValue += ( i * mLineByteS );
            }
            if( _tmpValue != 0 )
            {
                mCurrDisplayIndex = _tmpValue;
                sub_ChanageScrollValue();
                mCurrDisplayEndIndex = 0;
                mMainImageP->fill( mBackColor );
                sub_DataToImage();
                update();
            }
        }
    }
    else
    {
        if( mCurrDisplayIndex > 0 )
        {
            i = 3;
            _tmpValue = mCurrDisplayIndex;
            _tmpValue -= ( i * mLineByteS );
            while( _tmpValue < 0 )
            {
                i -= 1;
                if( i == 0 )
                {
                    _tmpValue = 0;
                    break;
                }
                _tmpValue = mCurrDisplayIndex;
                _tmpValue -= ( i * mLineByteS );
            }

            if( _tmpValue >= 0 )
            {
                mCurrDisplayIndex = _tmpValue;
                sub_ChanageScrollValue();
                mCurrDisplayEndIndex = 0;
                mMainImageP->fill( mBackColor );
                sub_DataToImage();
                update();
            }
        }
    }
}

/**
 * @brief SimFlashDrawControl::sub_ChanageScrollValue
 * @param pValue
 */
void SimFlashDrawControl::sub_ScrollBarChanage( qreal pValue )
{
    if( !mStopScrollChanageFlag )
    {

    }
    else
    {
        //qDebug() << "stop chanage";
        mStopScrollChanageFlag = false;
    }
}

/**
 * @brief SimFlashDrawControl::fun_CalcLineDisplayByteS
 * @param pWidth
 * @param pDataDisplayWidth
 * @param pAscDisplayWidth
 * @return
 *      一行最多显示的字节数(4的倍数)
 */
int SimFlashDrawControl::fun_CalcLineDisplayByteS( int pWidth, int pDataDisplayWidth, int pAscDisplayWidth )
{
    int _retValue;

    _retValue = 0;

    int i, j, _tmpU32Value;

    i = 4;
    j = 0;
    _tmpU32Value = i * pDataDisplayWidth + ( i - 1 ) * pAscDisplayWidth + i * pAscDisplayWidth;

    while( _tmpU32Value <= pWidth )
    {
        j = i;
        i *= 2;
        _tmpU32Value = i * pDataDisplayWidth + ( i - 1 ) * pAscDisplayWidth + i * pAscDisplayWidth;
    }

    if( j != 0 )
    {
        _retValue = j;
    }

    return _retValue;
}

/**
 * @brief SimFlashDrawControl::sub_DataToImag
 *      把数据绘制到IMAGE中
 */
void SimFlashDrawControl::sub_DataToImage()
{
    int _width;
    int _height;

    _width = width();
    _height = height();

    if( mMainImageP == nullptr )
    {
        mMainImageP = new QImage( _width, _height, QImage::Format_ARGB32 );
        mMainImageP->fill( mBackColor );
    }
    else
    {
        if( ( mMainImageP->width() != _width ) || ( mMainImageP->height() != _height ) )
        {
            delete mMainImageP;

            mMainImageP = new QImage( _width, _height, QImage::Format_ARGB32 );
            mMainImageP->fill( mBackColor );
        }
    }

    QPainter _tmpPainter( mMainImageP );
    //_tmpPainter.setRenderHint( QPainter::TextAntialiasing, true );
    //_tmpPainter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing, true );
    int _stringHeight;
    int _SignalDataWidth;
    int _SingleAscWidth;
    int _AddressStringWidth;
    int _colonStringWidth;
    QString _testString;

    QFontMetrics _tmpFm( mFont );

    _tmpPainter.setFont( mFont );

    _testString = "FF";
    _SignalDataWidth = _tmpFm.horizontalAdvance( _testString );
    _testString = "FFFFFFFF";
    _AddressStringWidth = _tmpFm.horizontalAdvance( _testString );
    _testString = ":";
    _colonStringWidth = _tmpFm.horizontalAdvance( _testString );
    _testString = "A";
    _SingleAscWidth = _tmpFm.horizontalAdvance( _testString );
    _stringHeight = _tmpFm.height();

    mFontHeight = _stringHeight;
    mDataFontWidth = _SignalDataWidth;
    mColonFontWidth = _colonStringWidth;
    mSingleAscWidth = _SingleAscWidth;
    mAddressStringWidth = _AddressStringWidth;

    int _tmpWidth = _width - _AddressStringWidth - _colonStringWidth - mSpaceValue - mSpaceValue - _colonStringWidth;
    int _lineByteS = fun_CalcLineDisplayByteS( _tmpWidth, _SignalDataWidth, _SingleAscWidth  );

    if( _lineByteS != 0 )
    {
        int _x, _y;
        int i, j;
        char * _tmpDisplayBufP;
        char _tmpTransferBuf[ 256 ];

        mLineByteS = _lineByteS;

        _y = mSpaceValue; //上边间隔

        _tmpDisplayBufP = new char [ _lineByteS ];

        j = 0;
        _y = 0;
        while( _y < _height )
        {
            _y += _stringHeight;
            i = fun_GetData( mCurrDisplayIndex + j, _lineByteS, _tmpDisplayBufP );
            if( i > 0 )
            {
                mCurrDisplayEndIndex += i;
                _x = mSpaceValue; //左边间隔

                sprintf( _tmpTransferBuf, "%08X", ( mCurrDisplayIndex + j ) );
                _testString = QString::fromStdString( _tmpTransferBuf );
                _tmpPainter.drawText( _x, _y, _testString );
                _x += _SignalDataWidth * 4;

                _testString = ":";
                _tmpPainter.drawText( _x, _y, _testString );
                _x += _colonStringWidth;

                for( int z = 0; z < i; z++ )
                {
                    sprintf( _tmpTransferBuf, "%02X", ( uint8_t )( _tmpDisplayBufP[ z ] ) );
                    _testString = QString::fromStdString( _tmpTransferBuf );
                    _tmpPainter.drawText( _x, _y, _testString );
                    _x += _SignalDataWidth;

                    _testString = " ";
                    _tmpPainter.drawText( _x, _y, _testString );
                    _x += _SingleAscWidth;
                }

                _x += _colonStringWidth;

                for( int z = 0; z < i; z++ )
                {
                    _tmpTransferBuf[ 0 ] = _tmpDisplayBufP[ z ];
                    _tmpTransferBuf[ 1 ] = 0;
                    _testString = QString::fromStdString( _tmpTransferBuf );
                    _tmpPainter.drawText( _x, _y, _testString );
                    _x += _SingleAscWidth;

                }
            }
            else
            {
                break;
            }
            j += i;
        }

        if( mCurrDisplayEndIndex != 0 )
        {
            mCurrDisplayEndIndex += mCurrDisplayIndex;
        }

        delete [] _tmpDisplayBufP;
    }
}

/**
 * @brief SimFlashDrawControl::sub_MouseDrag
 * @param pX
 * @param pY
 */
void SimFlashDrawControl::sub_MouseDrag( qreal pX, qreal pY )
{
    int _width, _height;

    if( mMainImageP != nullptr )
    {
        QPainter _tmpPainter( mMainImageP );
        QPen _tmpPen( Qt::red, 2 );

        _tmpPainter.begin( mMainImageP );
        _tmpPainter.setPen( _tmpPen );

        _tmpPainter.setRenderHint( QPainter::Antialiasing );

        if( mDragFlag == false )
        {
            mDragFlag = true;
            mCurrMouseX = pX;
            mCurrMouseY = pY;

            if( mDragSelectFlag )
            {
                mSaveDragRectImageP->save( "save.jpg" );

                _tmpPainter.drawImage( mSaveDragRect.x(), mSaveDragRect.y(), *mSaveDragRectImageP );

                mDragSelectFlag = false;

                delete mSaveDragRectImageP;
                mSaveDragRectImageP = nullptr;

                update( mSaveDragRect );
            }
        }
        else
        {
            int _startX, _startY;
            int _endX, _endY;

            if( pX < mCurrMouseX )
            {
                _startX = pX;
                _endX = mCurrMouseX;
            }
            else
            {
                _startX = mCurrMouseX;
                _endX = pX;
            }

            if( pY < mCurrMouseY )
            {
                _startY = pY;
                _endY = mCurrMouseY;
            }
            else
            {
                _startY = mCurrMouseY;
                _endY = pY;
            }

            _tmpPainter.setRenderHint( QPainter::Antialiasing );
            _width = _endX - _startX;
            _height = _endY - _startY;

            if( mSaveDragRectImageP != nullptr )
            {
                _tmpPainter.drawImage( mSaveDragRect.x(), mSaveDragRect.y(), *mSaveDragRectImageP );
                delete mSaveDragRectImageP;
                mSaveDragRectImageP = nullptr;

//                if( mPrevDragWidth > _width )
//                {
//                    update();
//                    return;
//                }
            }

            //if( ( _width > 20 ) && ( _height > 20 ) )
            {
                QRect _tmpRect( _startX, _startY, _width, _height );

                mSaveDragRectImageP = new QImage( _width + 6 , _height + 6, QImage::Format_ARGB32 );
                mSaveDragRect.setX( _tmpRect.x() - 2 );
                mSaveDragRect.setY( _tmpRect.y() - 2 );
                mSaveDragRect.setWidth( _width + 4 );
                mSaveDragRect.setHeight( _height + 4 );
                *mSaveDragRectImageP = mMainImageP->copy( _tmpRect.x() - 2, _tmpRect.y() - 2, _width + 4, _height + 4 );

                _tmpPainter.drawRect( _tmpRect );

//                if( mPrevDragWidth > _width )
//                {
//                    mSaveDragRectImageP->save( "save.jpg" );
//                    _tmpPainter.drawImage( mSaveDragRect.x(), mSaveDragRect.y(), *mSaveDragRectImageP );
//                }

                mPrevDragWidth = _width;
            }
        }

        _tmpPainter.end();
        update();
    }
}

/**
 * @brief SimFlashDrawControl::sub_MouseRelease
 */
void SimFlashDrawControl::sub_MouseRelease()
{
    if( mDragFlag )
    {
        mDragFlag = false;
        mDragSelectFlag = true;

        sub_AdjustDragSelectRect();
    }
}

/**
 * @brief SimFlashDrawControl::sub_AdjustDragSelectRect
 *
 */
void SimFlashDrawControl::sub_AdjustDragSelectRect()
{
    QRect _tmpRect;

    _tmpRect = mSaveDragRect;
    _tmpRect.setX( _tmpRect.x() + 2 );
    _tmpRect.setY( _tmpRect.y() + 2 );
    _tmpRect.setWidth( _tmpRect.width() - 4 );
    _tmpRect.setHeight( _tmpRect.height() - 4 );

    int _wdith = width();
    int _height = height();
    int _tmpValue;
    int _line;
    int _col;
    int _endLine;
    int _endCol;

    int _newX, _newY;
    int _newEndX, _newEndY;

    _tmpValue = _tmpRect.y() - mSpaceValue;
    _line = ( _tmpValue ) / mFontHeight;

    if( ( _line * mFontHeight ) < _tmpValue )
    {
        _line += 1;
        if( ( ( _line * mFontHeight ) - _tmpValue ) < 5 )
        {
            _line += 1;
        }
    }
    if( _line == 0 )
    {
        _line = 1;
    }

    qDebug() << "select line:" << _line;

    _newY = ( _line - 1 ) * mFontHeight + mSpaceValue + 2;
    _tmpRect.setY( _newY );

    _tmpValue = _tmpRect.x() - mSpaceValue;
    _tmpValue -= ( mAddressStringWidth + mColonFontWidth );

    _col = _tmpValue / ( mDataFontWidth + mSingleAscWidth );
    if( ( _col * ( mDataFontWidth + mSingleAscWidth ) ) < _tmpValue )
    {
        _col += 1;
        if( ( _col * ( mDataFontWidth + mSingleAscWidth ) - mSingleAscWidth ) < _tmpValue )
        {
            _col += 1;
        }
    }
    if( _col <= 0 )
    {
        _col = 1;
    }
    _newX = ( _col - 1 ) * ( mDataFontWidth + mSingleAscWidth ) + ( mAddressStringWidth + mColonFontWidth );
    _tmpRect.setX( _newX );

    qDebug() << "select col:" << _col;

    _tmpValue = _tmpRect.y() + _tmpRect.height() - mSpaceValue;
    _endLine = ( _tmpValue + ( mFontHeight / 2 ) ) / mFontHeight;
    if( _endLine == 0 )
    {
        _endLine = 1;
    }

    qDebug() << "select end line:" << _endLine;
    _newEndY = _endLine * mFontHeight + mSpaceValue + 2;
    _tmpRect.setHeight( _newEndY - _newY );

    _tmpValue = _tmpRect.x() + _tmpRect.width() - mSpaceValue;
    _tmpValue -= ( mAddressStringWidth + mColonFontWidth );

    _endCol = ( _tmpValue + ( ( mDataFontWidth + mSingleAscWidth ) / 2 ) ) / ( mDataFontWidth + mSingleAscWidth );
    if( _endCol <= 0 )
    {
        _endCol = 1;
    }

    qDebug() << "select end col:" << _endCol;
    _newEndX = _endCol * ( mDataFontWidth + mSingleAscWidth ) + ( mAddressStringWidth + mColonFontWidth );
    _tmpRect.setWidth( _newEndX - _newX );

    mSelectRect = _tmpRect;

    QPainter _tmpPainter( mMainImageP );
    QPen _tmpPen( Qt::darkGreen, 2 );
    _tmpPainter.setPen( _tmpPen );

    _tmpPainter.begin( mMainImageP );

    if( mSaveDragRectImageP != nullptr )
    {
        _tmpPainter.drawImage( mSaveDragRect.x(), mSaveDragRect.y(), *mSaveDragRectImageP );
        delete mSaveDragRectImageP;
    }

    mSaveDragRectImageP = new QImage( _tmpRect.width() + 6 , _tmpRect.height() + 6, QImage::Format_ARGB32 );
    mSaveDragRect.setX( _tmpRect.x() - 2 );
    mSaveDragRect.setY( _tmpRect.y() - 2 );
    mSaveDragRect.setWidth( _tmpRect.width() + 4 );
    mSaveDragRect.setHeight( _tmpRect.height() + 4 );
    *mSaveDragRectImageP = mMainImageP->copy( _tmpRect.x() - 2, _tmpRect.y() - 2,
                                              mSaveDragRect.width() + 4, mSaveDragRect.height() + 4 );
    _tmpPainter.drawRect( _tmpRect );

    _tmpPainter.end();

    update();
}

/**
 * @brief SimFlashDrawControl::sub_SlotReDraw
 */
void SimFlashDrawControl::sub_SlotReDraw()
{

}


/**
 * @brief SimFlashDrawControl::paint
 * @param pPainter
 */
void SimFlashDrawControl::paint( QPainter * pPainter )
{
    if( mMainImageP != nullptr )
    {
        pPainter->setRenderHint( QPainter::Antialiasing );
        pPainter->drawImage( 0, 0, *mMainImageP );
    }
}
