
#include "SimFlashListModelClass.h"

/**
 * @brief SimFlashListModelClass::SimFlashListModelClass
 * @param parent
 */
SimFlashListModelClass::SimFlashListModelClass( QObject * parent ) : QAbstractListModel( parent )
{
    int i;

    i = Qt::UserRole;

    mColumnRoleNameS.insert( i++, "flashAddress" );
    mColumnRoleNameS.insert( i++, "flashData" );
}

/**
 * @brief SimFlashListModelClass::~SimFlashListModelClass
 */
SimFlashListModelClass::~SimFlashListModelClass()
{

}


/******************************
 * 必须要重载的函数
 * ****************************/

QHash< int, QByteArray > SimFlashListModelClass::roleNames() const
{
    return mColumnRoleNameS;
}


int SimFlashListModelClass::rowCount( const QModelIndex &parent ) const
{
    return 0;
}


QVariant SimFlashListModelClass::data( const QModelIndex &index, int role ) const
{
    QVariant _ret;

    return _ret;
}

