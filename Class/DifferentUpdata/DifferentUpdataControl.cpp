
#include "../../Class/PrivateEventClass.h"
#include "../../Model/MainModelClass.h"
#include "DifferentUpdataControl.h"

DifferentUpdataControl::DifferentUpdataControl( QQuickItem * pParent ) : HexDataDisplayControl( pParent ), FileAnalyseBaseClass()
{
    mMainModelObjP = nullptr;
}

DifferentUpdataControl::~DifferentUpdataControl()
{

}

/**
 * @brief DifferentUpdataControl::sub_SetMainModelObj
 * @param pObjectP
 */
void DifferentUpdataControl::sub_SetMainModelObj( QObject * pObjectP )
{
    mMainModelObjP = ( MainModelClass * )pObjectP;

    PrivateEventClass * _tmpEventObjP;

    _tmpEventObjP = new PrivateEventClass( EventType_e::RegisterObject, DataType_e::DataType, Sender_e::DifferentUpdate, ( void * )this );
    mMainModelObjP->sub_ChildObjectEventHandle( ( void * )_tmpEventObjP );
}

/**
 * @brief DifferentUpdataControl::sub_DifferentFile
 * @param pFilePath
 */
void DifferentUpdataControl::sub_DifferentFile( QUrl pFilePath )
{
    if( pFilePath.isLocalFile() )
    {
        std::string _tmpStr;

        _tmpStr = pFilePath.path().toStdString();
    }
}
