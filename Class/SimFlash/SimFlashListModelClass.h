
#ifndef SIMFLASHLISTMODELCLASS_H
#define SIMFLASHLISTMODELCLASS_H

#include <QMap>
#include <QAbstractListModel>

class SimFlashListModelClass : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SimFlashListModelClass( QObject * parent = nullptr );
    virtual ~SimFlashListModelClass();

    void sub_AddContent( QString pAddressStr, QString pContentStr );

    /*********************************
     * 系统要求重载的函数
     * ******************************/
public:
    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    QHash< int, QByteArray > roleNames() const override;

private:
    QHash< int, QByteArray > mColumnRoleNameS;
    QMap< QString, QString > mModifyContentS;

};

#endif // SIMFLASHLISTMODELCLASS_H
