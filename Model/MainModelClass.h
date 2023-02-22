#ifndef MAINMODELCLASS_H
#define MAINMODELCLASS_H

#include <QObject>
#include <QUrl>
#include "../Class/SpiCaptureDataClass.h"
#include "./LogViewModel.h"
#include "../Control/GuiDrawControl.h"

class MainModelClass : public QObject
{
    Q_OBJECT
public:
    explicit MainModelClass(QObject *parent = nullptr);
    virtual ~MainModelClass();

    Q_INVOKABLE void sub_OpenSerialPortClick( QString pButtonText );
    Q_INVOKABLE void sub_AnalyseSpiClick( QUrl pSpiFilePath );
    Q_INVOKABLE void sub_ClearSelf();
    Q_INVOKABLE void sub_GetDrawObjectFromQml( QObject * pObjectP );

    void sub_ChildObjectEventHandle( void * pEventP );

public: //将变量申明
    LogViewModel * mLogViewModelObjP;

private:
    SpiCaptureDataClass * mSpiAnalyseObjP;

signals:

};

#endif // MAINMODELCLASS_H
