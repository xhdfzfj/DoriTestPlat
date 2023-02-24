#ifndef XHDLOGEVENTCLASS_H
#define XHDLOGEVENTCLASS_H

#include <QDateTime>


class xhdLogEventClass
{
public:
    enum LogLevel { logErr, logWarn, logInfo, logDebug };

public:
    explicit xhdLogEventClass();
    void sub_SetLogInfo( LogLevel pLevel, QString pInfo );

private:
    QDateTime mLogTime;
    LogLevel mLevel;
    QString mLogInfo;

public:
    QDateTime GetLogTime() { return mLogTime; }
    LogLevel GetLogLevel()  { return mLevel; }
    QString GetLogInfo()   { return mLogInfo; }
};

#endif // XHDLOGEVENTCLASS_H
