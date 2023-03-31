#ifndef TRANSIONEFFECT_H
#define TRANSIONEFFECT_H

#include <QObject>
#include <QImage>
#include <queue>
#include <mutex>
#include <list>

class TransionEffect : public QObject
{
    Q_OBJECT
public:
    explicit TransionEffect();
    virtual ~TransionEffect();

    QImage * fun_GetEffectImage();
    bool fun_AddEffectImage( QImage * pImageP );

private:
    std::mutex mMutex;
    std::queue< QImage * > mEffectImageS;
    std::list< QImage * > mDestoryImageS;
};

#endif // TRANSIONEFFECT_H
