#include "serialconfigmodel.h"
#include <QDebug>
#include <QSerialPortInfo>

SerialConfigModel::SerialConfigModel(QObject *parent):QObject(parent)
{
    refreshSerialNumList();
    connect(&serialPort,&QSerialPort::readyRead,this, &SerialConfigModel::DataReceived);
#if TEST_SERIAL
    connect(this,&SerialConfigModel::serialTest, this, &SerialConfigModel::testSerialSend);
#endif
}

SerialConfigModel::~SerialConfigModel()
{
#if TEST_SERAL
    if(timer)
    {
        timer->stop();
        timer->deleteLater();
        timer = nullptr;
    }
#endif
    if(serialPort.isOpen())
    {
        serialPort.close();
    }
//    if(mSerialThreadP)
//    {
//        mSerialThreadP->join();
//        delete mSerialThreadP;
//        mSerialThreadP = nullptr;
//    }

}

void SerialConfigModel::refreshSerialNumList()
{
    m_serialNumList.clear();
    m_serialNumList.append("刷新");
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        m_serialNumList.append(info.portName());
    }
    if(m_serialNumList.size()==1)
    {
        m_serialNumList.append("");
    }
    emit serialNumListChanged();
}

void SerialConfigModel::DataReceived()
{
    QByteArray arr = serialPort.readAll();                      // 读取数据
    qDebug() << "serial Receive";
    if(!arr.isEmpty())
    {
        emit serialDataReady(arr);
    }

}

void SerialConfigModel::onBaudComboBoxSelect(int currentIndex)
{

    currentBaudrate = m_baudList.at(currentIndex);

}

void SerialConfigModel::onSerialComboBoxSelect(int currentIndex)
{
    if(currentIndex == 0)
    {
        refreshSerialNumList();
        return;
    }
    currentSerialNum = m_serialNumList.at(currentIndex);
}

QStringList SerialConfigModel::baudList() const
{
    return m_baudList;
}

QStringList SerialConfigModel::serialNumList() const
{
    return m_serialNumList;
}

int SerialConfigModel::serialStatus()
{
    return serialPort.isOpen();
}

#if TEST_SERIAL
void SerialConfigModel::testSerialSend()
{
    if(timer != nullptr)
        return;
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,[&](){
        if(serialPort.isOpen())
        {
            serialPort.write("ABC_123");
            qDebug() << "serial Send";
        }

    });
    timer->setInterval(1000);
    timer->start();
}
#endif

void SerialConfigModel::openSerialPort()
{
//    if(mSerialThreadP)
//    {
//        mSerialThreadP->join();
//        delete mSerialThreadP;
//        mSerialThreadP = nullptr;
//    }
//    mSerialThreadP = new std::thread( &SerialConfigModel::openSerialRun, this );
      openSerialRun();
}

void SerialConfigModel::openSerialRun()
{

    qDebug()<<"Serial: "<<currentSerialNum << " " << currentBaudrate;
    if(currentSerialNum.isEmpty())
    {
        return;
    }
    if(serialPort.isOpen())
    {
        serialPort.close();
        qDebug() << "closeSerial";
    }else{
        qDebug() << currentSerialNum << " " << currentBaudrate;
        serialPort.setPortName(currentSerialNum);
        serialPort.setBaudRate(currentBaudrate.toInt());
        if(serialPort.open(QIODevice::ReadWrite))
        {
            qDebug() << "openSerial";
#if TEST_SERIAL
            emit serialTest();
#endif

        }else{
            qDebug() << "openSerial Fail";
        }

    }
    emit serialStatusChanged();

}



