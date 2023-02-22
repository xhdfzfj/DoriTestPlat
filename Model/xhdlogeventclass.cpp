#include "xhdlogeventclass.h"

/**
 * @brief xhdLogEventClass::xhdLogEventClass
 *      构造函数
 */
xhdLogEventClass::xhdLogEventClass()
{
    mLogTime = QDateTime::currentDateTime();
}

/**
 * @brief xhdLogEventClass::sub_SetLogInfo
 * @param pLevel
 * @param pInfo
 */
void xhdLogEventClass::sub_SetLogInfo( LogLevel pLevel, QString pInfo )
{
    mLevel = pLevel;
    mLogInfo = pInfo;
}
