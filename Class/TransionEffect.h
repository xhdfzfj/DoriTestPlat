#ifndef TRANSIONEFFECT_H
#define TRANSIONEFFECT_H

#include <QObject>
#include <QImage>
#include <queue>
#include <mutex>
#include <list>
#include <functional>

class TransionEffect : public QObject
{
    Q_OBJECT
public:
    explicit TransionEffect();
    virtual ~TransionEffect();

    QImage * fun_GetEffectImage();
    bool fun_AddEffectImage( QImage * pImageP );
    bool fun_SetUpLevelInterface( std::function< int( void * )> pInf );

private:
    std::mutex mMutex;
    std::queue< QImage * > mEffectImageS;
    std::list< QImage * > mDestoryImageS;
    std::function< int( void * ) > mUpLevelInf;

};

#endif // TRANSIONEFFECT_H
