#include <QDebug>
#include <mutex>

#include "spiviewmodel.h"

std::mutex mCmdInfoMutex;

/**
 * @brief SpiViewModel::SpiViewModel
 * @param parent
 */
SpiViewModel::SpiViewModel(QObject * parent ) : QAbstractTableModel( parent )
{
    mCmdInfoCount = 0;
}

/**
 * @brief SpiViewModel::~SpiViewModel
 */
SpiViewModel::~SpiViewModel()
{

}

/**
 * @brief SpiViewModel::sub_EnterSpiCmdInfo
 * @param pSpiCmdInfoP
 */
void SpiViewModel::sub_EnterSpiCmdInfo( SpiCmdInfoClass *pSpiCmdInfoP )
{
    SpiCmdInfoClass * _tmpSpiCmdInfoP;
    int _tmpValue;

    //由于传入的参数，在调用处会被释放
    _tmpSpiCmdInfoP = nullptr;
    _tmpSpiCmdInfoP = pSpiCmdInfoP->Clone();

    mCmdInfoMutex.lock();

    _tmpValue = mCmdInfoS.size();

    mCmdInfoS.insert( std::pair< int, SpiCmdInfoClass * >( mCmdInfoCount, _tmpSpiCmdInfoP ) );
    mCmdInfoCount += 1;

    qDebug() << "mCmdInfoCount:" << mCmdInfoCount;

    mCmdInfoMutex.unlock();

    beginInsertRows( QModelIndex(), _tmpValue, _tmpValue + 1 );
    endInsertRows();

}


/*****************************************************
 * 基类要求重载的函数
 * ***************************************************/

int SpiViewModel::columnCount( const QModelIndex &parent ) const
{
    if( parent.isValid() )
        return 0;
    return 2;
}


int SpiViewModel::rowCount( const QModelIndex &parent ) const
{
    int _retValue;

    _retValue = 0;

    if (parent.isValid())
    {
        _retValue = 0;
    }
    else
    {
        _retValue = mCmdInfoS.size();
    }
    return _retValue;
}


QHash< int, QByteArray > SpiViewModel::roleNames() const
{
    QHash< int, QByteArray > _roleNameS;

    _roleNameS[ 0 ] = "display";
    //_roleNameS[ 1 ] = "Level";
    //_roleNameS[ 2 ] = "Info";

    return _roleNameS;
}


QVariant SpiViewModel::headerData( int select, Qt::Orientation orientation, int role ) const
{
    if( role != Qt::DisplayRole )
    {
        return QVariant();
    }

    if( select == 0 )
    {
        return "SPI指令";
    }
    else if( select == 1 )
    {
        return "数据";
    }
    else
    {
        return QVariant();
    }
}


QVariant SpiViewModel::data(const QModelIndex &index, int role) const
{
    SpiCmdInfoClass * _spiInfoP;

    if( !index.isValid() )
    {
        return QVariant();
    }

    if( role == Qt::DisplayRole )
    {
        if( ( index.row() >= 0 ) && ( index.row() < rowCount() ) &&
            ( index.column() >= 0 ) && ( index.column() < columnCount() ) )
        {
            QString _tmpStr;
            int _rowIndex;
            int _colIndex;
            int _collectIndex;

            _rowIndex = index.row();
            _colIndex = index.column();

            _collectIndex = _rowIndex;

            mCmdInfoMutex.lock();
            if( mCmdInfoS.count( _collectIndex ) )
            {
                _spiInfoP = mCmdInfoS.at( _collectIndex );
                mCmdInfoMutex.unlock();

                if( _colIndex == 0 )
                {
                    _tmpStr = QString::number( _spiInfoP->mCmd, 16 );
                    _tmpStr += ":" + QString::fromStdString( _spiInfoP->mCmdInfo );
                    return QVariant( _tmpStr );
                }
            }
            else
            {
                mCmdInfoMutex.unlock();
            }
        }
    }

    return QVariant();

}
