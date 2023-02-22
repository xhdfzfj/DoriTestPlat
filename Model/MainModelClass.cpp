#include <QDebug>
#include "MainModelClass.h"

/**
 * @brief MainModelClass::MainModelClass
 * @param parent
 */
MainModelClass::MainModelClass(QObject *parent)
    : QObject{parent}
{
    mLogViewModelObjP = new LogViewModel( this );
    mSpiAnalyseObjP = nullptr;
}

/**
 * @brief MainModelClass::~MainModelClass
 */
MainModelClass::~MainModelClass()
{
    delete mLogViewModelObjP;
}

/**
 * @brief MainModelClass::sub_ClearSelf
 *      自清理
 */
void MainModelClass::sub_ClearSelf()
{
   delete this;
}

/**
 * @brief MainModelClass::sub_OpenSerialPortClick
 *      打开串按键
 */
void MainModelClass::sub_OpenSerialPortClick( QString pButtonText )
{
    mLogViewModelObjP->sub_EnterLog( xhdLogEventClass::LogLevel::logInfo, pButtonText );
}

/**
 * @brief MainModelClass::sub_AnalyseSpiClick
 *      开始分析SPI捕捉的文件
 * @param pSpiFilePath
 */
void MainModelClass::sub_AnalyseSpiClick( QUrl pSpiFilePath )
{
    if( pSpiFilePath.isLocalFile() )
    {
        std::string _tmpStr;

        _tmpStr = pSpiFilePath.path().toStdString();

        if( mSpiAnalyseObjP != nullptr )
        {
            delete mSpiAnalyseObjP;
            mSpiAnalyseObjP = nullptr;
        }

        mSpiAnalyseObjP = new SpiCaptureDataClass( _tmpStr );
    }
    else
    {
        mLogViewModelObjP->sub_EnterLog( xhdLogEventClass::LogLevel::logErr, "请选择SPI文件" );
    }
}
