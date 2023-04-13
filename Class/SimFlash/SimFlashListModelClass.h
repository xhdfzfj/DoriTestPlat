
#ifndef SIMFLASHLISTMODELCLASS_H
#define SIMFLASHLISTMODELCLASS_H


#include <QAbstractListModel>

class SimFlashListModelClass : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SimFlashListModelClass( QObject * parent = nullptr );
    virtual ~SimFlashListModelClass();

    /*********************************
     * 系统要求重载的函数
     * ******************************/
public:
    int rowCount( const QModelIndex &parent ) const;
    QVariant data( const QModelIndex &index, int role ) const;
    QHash< int, QByteArray > roleNames() const;

private:
    QHash< int, QByteArray > mColumnRoleNameS;
};

#endif // SIMFLASHLISTMODELCLASS_H
