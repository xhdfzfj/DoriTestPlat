#ifndef SERIALCONFIGMODEL_H
#define SERIALCONFIGMODEL_H

#define TEST_SERIAL 0

#include <QObject>
#include <QSerialPort>

//#include <thread>
#if TEST_SERIAL
#include <QTimer>
#endif

class SerialConfigModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList baudList READ baudList NOTIFY baudListChanged);
    Q_PROPERTY(QStringList serialNumList READ serialNumList NOTIFY serialNumListChanged);
    Q_PROPERTY(int serialStatus READ serialStatus NOTIFY serialStatusChanged)
public:
    explicit SerialConfigModel(QObject *parent = nullptr);
    ~SerialConfigModel();
    Q_INVOKABLE void onBaudComboBoxSelect(int currentIndex);
    Q_INVOKABLE void onSerialComboBoxSelect(int currentIndex);
    QStringList baudList() const;
    QStringList serialNumList() const;
    int serialStatus();
    void openSerialPort();

signals:
    void baudListChanged();
    void serialNumListChanged();
    void serialStatusChanged();
    void serialDataReady(QByteArray);
#if TEST_SERIAL
    void serialTest();
#endif
private:
    QStringList m_baudList = {"115200","9600","4800"};
    QStringList m_serialNumList = {"刷新"};
    QString currentBaudrate;
    QString currentSerialNum;
    QSerialPort serialPort;
    //std::thread * mSerialThreadP = nullptr;

    void openSerialRun();
#if TEST_SERIAL
    QTimer *timer = nullptr;
#endif
    void refreshSerialNumList();

private slots:
    void DataReceived();
#if TEST_SERIAL
    void testSerialSend();
#endif

};

#endif // SERIALCONFIGMODEL_H
