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
    }

    return _retP;
}
