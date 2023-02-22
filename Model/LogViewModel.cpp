#include "LogViewModel.h"

std::mutex mLogMutex;

/**
 * @brief LogViewModel::LogViewModel
 * @param parent
 */
LogViewModel::LogViewModel(QObject * parent ) : QAbstractTableModel( parent )
{
    mlogCount = 0;
    mlogInverseCount = 0;
}

/**
 * @brief LogViewModel::~LogViewModel
 */
LogViewModel::~LogViewModel()
{
    if( !mLogCollect.empty() )
    {
        xhdLogEventClass * _tmpLogObjP;
        std::map< int, xhdLogEventClass * >::iterator _itm;

        for( _itm = mLogCollect.begin(); _itm != mLogCollect.end(); _itm++ )
        {
            _tmpLogObjP = _itm->second;
            delete _tmpLogObjP;
        }
    }
}

/* @brief LogViewModel::sub_EnterLog
*      LOG信息输入
* @param pLevel
* @param pInfo
*/
void LogViewModel::sub_EnterLog( xhdLogEventClass::LogLevel pLevel, QString pInfo )
{
   int _tmpValue;

   xhdLogEventClass * _tmpLogObjP;

   _tmpValue = ( int )pLevel;
   //if( mDisplayItemFlagS[ _tmpValue ] )
   {
       //pthread_mutex_lock( &mLogMutex );

       mLogMutex.lock();
       _tmpValue = mLogCollect.size();

       _tmpLogObjP = new xhdLogEventClass();
       if( _tmpLogObjP != nullptr )
       {
           _tmpLogObjP->sub_SetLogInfo( pLevel, pInfo );
           mLogCollect.insert( std::pair< int, xhdLogEventClass * >( mlogCount, _tmpLogObjP ) );
           mlogCount += 1;
       }
       mLogMutex.unlock();
       //pthread_mutex_unlock( &mLogMutex );

       beginInsertRows( QModelIndex(), _tmpValue, _tmpValue + 1 );
       endInsertRows();
   }
}


/*****************************************************
 * 基类要求重载的函数
 * ***************************************************/

int LogViewModel::columnCount( const QModelIndex &parent ) const
{
    if( parent.isValid() )
        return 0;
    return 3;
}


int LogViewModel::rowCount( const QModelIndex &parent ) const
{
    int _retValue;

    if (parent.isValid())
    {
        _retValue = 0;
    }
    else
    {
        _retValue = mLogCollect.size();
    }
    return _retValue;
}


QHash< int, QByteArray > LogViewModel::roleNames() const
{
    QHash< int, QByteArray > _roleNameS;

    _roleNameS[ 0 ] = "display";
    //_roleNameS[ 1 ] = "Level";
    //_roleNameS[ 2 ] = "Info";

    return _roleNameS;
}


QVariant LogViewModel::headerData( int select, Qt::Orientation orientation, int role ) const
{
    if( role != Qt::DisplayRole )
    {
        return QVariant();
    }

    if( select == 0 )
    {
        return "时间";
    }
    else if( select == 1 )
    {
        return "等级";
    }
    else if( select == 2 )
    {
        return "信息";
    }
    else
    {
        return QVariant();
    }
}


QVariant LogViewModel::data(const QModelIndex &index, int role) const
{
    xhdLogEventClass * _logObjP;

    if( !index.isValid() )
    {
        return QVariant();
    }
    if( role == Qt::DisplayRole )
    {
        if( ( index.row() >= 0 ) && ( index.row() < rowCount() ) &&
            ( index.column() >= 0 ) && ( index.column() < columnCount() ) )
        {
            int _rowIndex;
            int _colIndex;
            int _collectIndex;

            _rowIndex = index.row();
            _colIndex = index.column();
            _collectIndex = _rowIndex + mlogInverseCount;

            //pthread_mutex_lock( &mLogMutex );
            mLogMutex.lock();
            if( mLogCollect.count( _collectIndex ) )
            {
                _logObjP = mLogCollect.at( _collectIndex );

                //pthread_mutex_unlock( &mLogMutex );
                mLogMutex.unlock();

                if( _colIndex == 0 )
                {
                    return QVariant( _logObjP->GetLogTime().toString() );
                }
                else if( _colIndex == 1 )
                {
                    switch( _logObjP->GetLogLevel() )
                    {
                        case xhdLogEventClass::LogLevel::logErr:
                            return QVariant( "错误" );
                            break;
                        case xhdLogEventClass::LogLevel::logWarn:
                            return QVariant( "警告" );
                            break;
                        case xhdLogEventClass::LogLevel::logInfo:
                            return QVariant( "信息" );
                            break;
                        case xhdLogEventClass::LogLevel::logDebug:
                            return QVariant( "调试" );
                            break;
                    }
                }
                else if( _colIndex == 2 )
                {

                    return QVariant( _logObjP->GetLogInfo() );
                }
                else
                {

                    return QVariant();
                }
            }
            else
            {
                //pthread_mutex_unlock( &mLogMutex );
                mLogMutex.unlock();
                return QVariant();
            }

        }
        else
        {
            return QVariant();
        }
    }

    return QVariant();
}

