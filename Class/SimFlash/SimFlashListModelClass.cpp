
#include <mutex>
#include "SimFlashListModelClass.h"

std::mutex mModifyMutex;

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

/**
 * @brief SimFlashListModelClass::sub_AddContent
 * @param pAddressStr
 * @param pContentStr
 */
void SimFlashListModelClass::sub_AddContent( QString pAddressStr, QString pContentStr )
{
    int _tmpValue;

    if( !mModifyContentS.count( pAddressStr ) )
    {
        mModifyMutex.lock();
        _tmpValue = mModifyContentS.size();
        mModifyContentS[ pAddressStr ] = pContentStr;
        mModifyMutex.unlock();
        beginInsertRows( QModelIndex(), _tmpValue, _tmpValue );
        endInsertRows();

//        QVector< int > _tmpRole;

//        _tmpRole.push_back( Qt::UserRole );
//        _tmpRole.push_back( Qt::UserRole + 1 );
//        emit dataChanged( createIndex( 0, 0 ), createIndex( mModifyContentS.size() - 1, 0 ), _tmpRole );
    }
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
    Q_UNUSED(parent);
    return mModifyContentS.size();
}


QVariant SimFlashListModelClass::data( const QModelIndex &index, int role ) const
{
    QVariant _ret;
    int _index;
    int i;
    QString _tmpStr;

    _index = index.row();
    if( role == Qt::UserRole )
    {
        mModifyMutex.lock();
        i = 0;
        for( QMap< QString, QString >::key_iterator _ite = mModifyContentS.keyBegin();
             _ite != mModifyContentS.keyEnd(); _ite++ )
        {
            if( i == _index )
            {
                _tmpStr =  *_ite;
                _ret.setValue( _tmpStr );
                break;
            }
            i += 1;
        }
        mModifyMutex.unlock();
    }
    else if( role == ( Qt::UserRole + 1 ) )
    {
        mModifyMutex.lock();
        i = 0;
        for( QMap< QString, QString >::key_iterator _ite = mModifyContentS.keyBegin();
             _ite != mModifyContentS.keyEnd(); _ite++ )
        {
            if( i == _index )
            {
                _tmpStr = mModifyContentS[ *_ite ];
                _ret.setValue( _tmpStr );
                break;
            }
            i += 1;
        }
        mModifyMutex.unlock();
    }

    return _ret;
}

