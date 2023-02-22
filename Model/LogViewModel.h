#ifndef LOGVIEWMODEL_H
#define LOGVIEWMODEL_H

#include <QAbstractTableModel>
#include <mutex>
#include "./xhdlogeventclass.h"

class LogViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit LogViewModel( QObject * pparent = nullptr );
    virtual ~LogViewModel();

    void sub_EnterLog(xhdLogEventClass::LogLevel pLevel, QString pInfo);

    /**************************
     * 变量
     * ***********************/
public:


private:
    std::map< int, xhdLogEventClass * > mLogCollect;
    int mlogCount;          //正向计数值
    int mlogInverseCount;   //反向计数值

    /**************************
     * 基类要求重载的函数
     * ************************/
public:
    Q_INVOKABLE QVariant headerData( int select, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;
    QHash< int, QByteArray > roleNames() const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // LOGVIEWMODEL_H
