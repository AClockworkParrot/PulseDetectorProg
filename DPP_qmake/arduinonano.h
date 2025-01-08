#ifndef ARDUINONANO_H
#define ARDUINONANO_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class ArduinoNano : public QObject
{
    Q_OBJECT
public:

    explicit ArduinoNano(QObject *parent = nullptr);
    //Открывает последовательный порт, используя имя порта (portName).
    void start(QString portName);
    //Закрывает последовательный порт,
    void stop();
    // AGC — Automatic Gain Control
    void agcSet(int koeff);

signals:
    void koeffChanged(int k);
    void dataChanged(int a, int b, int c, int d);

    // обработка данных конечным автоматом
private slots:
    void onDataReceived();

private:

    enum States{
        WAIT_START, // стартовый байт
        WAIT_CMD1, // байт адреса
        WAIT_CMD0, // байт адреса
        WAIT_DATA8, // байт значения коэффициента усиления
        WAIT_DATA7, // байт первого счетчика
        WAIT_DATA6, // байт первого счетчика
        WAIT_DATA5, // байт второго счетчика
        WAIT_DATA4, // байт второго счетчика
        WAIT_DATA3, // байт третьего счетчика
        WAIT_DATA2, // байт третьего счетчика
        WAIT_DATA1, // байт четвертого счетчика
        WAIT_DATA0, // байт четвертого счетчика
        WAIT_CRC
    };

    //Объект, представляющий последовательный порт.
    QSerialPort port;
    // переменная состояния машины
    int state;
};

#endif // ARDUINONANO_H
