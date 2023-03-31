#include "TransionEffect.h"

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
    }
    else
    {
        _retP = mEffectImageS.front();
        mEffectImageS.pop();

        mDestoryImageS.push_back( _retP );

        if( mDestoryImageS.size() > 50 )
        {
            std::list< QImage * >::iterator _itm;
            QImage * _tmpImageP;

            for( _itm = mDestoryImageS.begin(); _itm != mDestoryImageS.end(); _itm++ )
            {
                _tmpImageP = *_itm;
                delete _tmpImageP;
            }
        }
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
