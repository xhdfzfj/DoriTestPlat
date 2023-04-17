#include "TransionEffect.h"
#include "PrivateEventClass.h"
#include <QPainter>

TransionEffect::TransionEffect()
{

}


TransionEffect::~TransionEffect()
{
    QImage * _tmpImageP;

    if( !mEffectImageS.empty() )
    {
        while( !mEffectImageS.empty() )
        {
            _tmpImageP = mEffectImageS.front();
            mEffectImageS.pop();
            delete _tmpImageP;
        }
    }

    if( !mDestoryImageS.empty() )
    {
        std::list< QImage * >::iterator _itm;

        for( _itm = mDestoryImageS.begin(); _itm != mDestoryImageS.end(); _itm++ )
        {
            _tmpImageP = *_itm;
            delete _tmpImageP;
        }

        mDestoryImageS.clear();
    }

    if( !mDestoryAsyncTimerS.empty() )
    {
        std::list< AsyncTimerClass * >::iterator _itm;
        AsyncTimerClass * _tmpTimerP;

        for( _itm = mDestoryAsyncTimerS.begin(); _itm != mDestoryAsyncTimerS.end(); _itm++ )
        {
            _tmpTimerP = *_itm;
            delete _tmpTimerP;
        }

        mDestoryAsyncTimerS.clear();
    }
}

/**
 * @brief TransionEffect::fun_AddEffectImage
 * @param pImageP
 * @return
 */
bool TransionEffect::fun_AddEffectImage( QImage * pImageP )
{
    std::lock_guard< std::mutex > _lock( mMutex );

    mEffectImageS.push( pImageP );

    return true;
}

/**
 * @brief TransionEffect::fun_GetEffectImage
 * @return
 */
QImage * TransionEffect::fun_GetEffectImage()
{
    QImage * _retP;
    std::lock_guard< std::mutex > _lock( mMutex );

    if( mEffectImageS.empty() )
    {
        _retP = nullptr;
        if( !mDestoryAsyncTimerS.empty() )
        {
            std::list< AsyncTimerClass * >::iterator _itm;
            AsyncTimerClass * _tmpTimerP;

            for( _itm = mDestoryAsyncTimerS.begin(); _itm != mDestoryAsyncTimerS.end(); _itm++ )
            {
                _tmpTimerP = *_itm;
                delete _tmpTimerP;
            }

            mDestoryAsyncTimerS.clear();
        }
    }
    else
    {
        _retP = mEffectImageS.front();
        mEffectImageS.pop();

        PrivateEventClass * _tmpEventObjP = new PrivateEventClass( EventType_e::GuiTransionEffect, DataType_e::StringType, "");
        AsyncTimerClass * _tmpAsync = new AsyncTimerClass( mWaitMilliSecond, mUpLevelInf, ( void * )_tmpEventObjP );

        mDestoryAsyncTimerS.push_back( _tmpAsync );

        if( mDestoryImageS.size() > 50 )
        {
            std::list< QImage * >::iterator _itm;
            QImage * _tmpImageP;

            for( _itm = mDestoryImageS.begin(); _itm != mDestoryImageS.end(); _itm++ )
            {
                _tmpImageP = *_itm;
                delete _tmpImageP;
            }

            mDestoryImageS.clear(); 
        }

        mDestoryImageS.push_back( _retP );
    }

    return _retP;
}

/**
 * @brief TransionEffect::fun_SetUpLevelInterface
 * @param pInf
 * @return
 */
bool TransionEffect::fun_SetUpLevelInterface( std::function< int( void * )> pInf )
{
    mUpLevelInf = pInf;

    //mUpLevelInf( nullptr );
    return true;
}

/**
 * @brief TransionEffect::fun_CreateEffect
 *      生成效果
 * @param pMainImageP
 *      主显示图
 * @param pEffectImageP
 *      叠加图
 * @param pInMainImage
 *      叠加图在主图中最终位置
 * @param pFrameCount
 *      效果帧数
 * @param pEffect
 *      1 代表边框扩散
 * @return
 */
bool TransionEffect::fun_CreateEffect( QImage * pMainImageP, QImage * pEffectImageP, QPoint pInMainImage, int pFrameCount, int pEffect, int pEffectTimeLen )
{
    if( pEffect == 1 )
    {
        sub_CreateBorderExpandEffect( pMainImageP, pEffectImageP, pInMainImage, pFrameCount );
        mWaitMilliSecond = pEffectTimeLen / pFrameCount;
        if( mWaitMilliSecond == 0 )
        {
            mWaitMilliSecond = 1;
        }
    }
    else if( pEffect == 2 )
    {
        sub_CreateExplosionEffect( pMainImageP, pEffectImageP, pInMainImage, pFrameCount );
        mWaitMilliSecond = pEffectTimeLen / pFrameCount;
        if( mWaitMilliSecond == 0 )
        {
            mWaitMilliSecond = 1;
        }
    }

    return true;
}

/**
 * @brief TransionEffect::sub_CreateBorderExpandEffect
 * @param pMainImageP
 * @param pEffectImageP
 * @param pInMainImage
 * @param pFrameCount
 */
void TransionEffect::sub_CreateBorderExpandEffect( QImage * pMainImageP, QImage * pEffectImageP, QPoint pInMainImage, int pFrameCount )
{
    int _width;
    int _height;
    QImage * _tmpImageP;
    QRect _tmpRect;
    QPen _tmpPen( Qt::red, 2 );
    QColor _tmpColor;

    _width = pEffectImageP->width();
    _height = pEffectImageP->height();

    _width /= pFrameCount;
    _height /= pFrameCount;

    _tmpColor = pEffectImageP->pixelColor( 0, 0 );
    _tmpColor.setAlpha( 255 );
    _tmpPen.setColor( _tmpColor );

    for( int i = 0; i < pFrameCount; i++ )
    {
        _tmpImageP = new QImage( pMainImageP->width(), pMainImageP->height(), QImage::Format_ARGB32 );
        *_tmpImageP = pMainImageP->copy( 0, 0, pMainImageP->width(), pMainImageP->height() );
        _tmpRect = QRect( pInMainImage.x(), pInMainImage.y(), _width * ( i + 1 ), _height * ( i + 1 ) );

        QPainter _tmpPainter( _tmpImageP );
        _tmpPainter.begin( _tmpImageP );
        _tmpPainter.setPen( _tmpPen );
        _tmpPainter.drawRect( _tmpRect );
        _tmpPainter.end();

        //_tmpImageP->save( "test" + QString::number( i ) + ".jpg" );

        fun_AddEffectImage( _tmpImageP );
    }
}

/**
 * @brief TransionEffect::sub_CreateExplosionEffect
 *      模拟一个爆炸效果
 * @param pMainImageP
 * @param pEffectImageP
 * @param pInMainImage
 * @param pFrameCount
 */
void TransionEffect::sub_CreateExplosionEffect( QImage * pMainImageP, QImage * pEffectImageP, QPoint pInMainImage, int pFrameCount )
{
    QRect _tmpRect;
    int _tmpValue;

    _tmpRect = QRect( pInMainImage.x(), pInMainImage.y(), pEffectImageP->width(), pEffectImageP->height() );
}
