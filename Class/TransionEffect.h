#ifndef TRANSIONEFFECT_H
#define TRANSIONEFFECT_H

#include <QObject>
#include <QImage>
#include <QPen>
#include <queue>
#include <mutex>
#include <list>
#include <functional>
#include "./AsyncTimerClass.h"

class TransionEffect : public QObject
{
    Q_OBJECT
public:
    explicit TransionEffect();
    virtual ~TransionEffect();

    QImage * fun_GetEffectImage();
    bool fun_AddEffectImage( QImage * pImageP );
    bool fun_SetUpLevelInterface( std::function< int( void * )> pInf );

    bool fun_CreateEffect( QImage * pMainImageP, QImage * pEffectImageP, QPoint pInMainImage, int pFrameCount, int pEffect, int pEffectTimeLen = 1000 );

private:
    std::mutex mMutex;
    std::queue< QImage * > mEffectImageS;
    std::list< QImage * > mDestoryImageS;
    std::function< int( void * ) > mUpLevelInf;

    int mWaitMilliSecond;

    std::list< AsyncTimerClass * > mDestoryAsyncTimerS;

private:
    void sub_CreateBorderExpandEffect( QImage * pMainImageP, QImage * pEffectImageP, QPoint pInMainImage, int pFrameCount );

};

#endif // TRANSIONEFFECT_H
