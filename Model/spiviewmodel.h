#ifndef SPIVIEWMODEL_H
#define SPIVIEWMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <map>
#include "../Include/CommonStruct.h"
#include "../Class/SpiCmdInfoClass.h"

class SpiViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SpiViewModel( QObject * pparent = nullptr );
    virtual ~SpiViewModel();

    void sub_EnterSpiCmdInfo( SpiCmdInfoClass * pSpiCmdInfoP );
private:
    std::map< int, SpiCmdInfoClass * > mCmdInfoS;
    int mCmdInfoCount;

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

#endif // SPIVIEWMODEL_H
