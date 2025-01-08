#include "arduinonano.h"
#include "QDebug"


//Конструктор класса, вызывающий метод stop() для сброса состояния и завершения любых открытых соединений.
ArduinoNano::ArduinoNano(QObject *parent):QObject(parent)
{
    this->stop();
}


// Метод START открывает соединение с указанным последовательным портом.
void ArduinoNano::start(QString portName)
{
    // Устанавливает имя порта (например, «COM4»).
    this->port.setPortName(portName);
    // Устанавливает скорость передачи данных (9600 бод).
    this->port.setBaudRate(QSerialPort::Baud9600);
    // Устанавливает отсутствие контроля чётности.
    this->port.setParity(QSerialPort::NoParity);
    // Указывает, что используются два стоп-бита.
    this->port.setStopBits(QSerialPort::TwoStop);
    // Проверка, открыт ли порт.
    if(!this->port.isOpen()) {
        // Если он закрыт, порт открывается в режиме чтения-записи
        bool result = this->port.open(QIODevice::ReadWrite);
        if(result) {
            //Если открытие успешно:
            //Логируется сообщение "connect success".
            //Сигнал readyRead (новые данные получены) связывается с методом onDataReceived.
            qDebug() << "connect success";
            connect(&this->port, &QSerialPort::readyRead, this, &ArduinoNano::onDataReceived);
        }
        //Если открытие не удалось, выводится "connect fail".
        else {
            qDebug() << "connect fail";
        }
    }
}


// Метод STOP
void ArduinoNano::stop()
{
    //Закрывает соединение:
    //Если порт открыт, отключает сигнал readyRead от метода onDataReceived.
    if(this->port.isOpen()){
        disconnect(&this->port, &QSerialPort::readyRead, this, &ArduinoNano::onDataReceived);
        //Закрывает порт.
        this->port.close();
    }
}


// Метод agcSet oтправляет команду для установки коэффициента усиления:
void ArduinoNano::agcSet(int koeff)
{

}


// метод onDataReceived Обрабатывает входящие данные с помощью конечного автомата.
void ArduinoNano::onDataReceived()
{
    //Общие переменные
    //state: Текущее состояние автомата (по умолчанию WAIT_START).
    static int state = States::WAIT_START;
    // cmd: Переменная для хранения команды (2 байта).
    static uint16_t cmd;
    //hh, mm, ss: Время (часы, минуты, секунды).
    static int a, b, c, d, k;

    // вывод в консоль оповещения о получении данных
    qDebug() << "data received";
    // Чтение данных:
    //Пока данные доступны (bytesAvailable), читается по одному байту.
    while(this->port.bytesAvailable()) {
        unsigned char byte;
        this->port.read((char*)&byte, 1);

        // Переход между состояниями:
        switch(state) {
        //Ожидается начальный байт 0xE7. При получении переход к WAIT_CMD1.
        case States::WAIT_START: {
            qDebug() << "wait start state";
            if(byte ==0xE7)
                state = States::WAIT_CMD1;
        } break;
            //Сбор команды (2 байта).
        case States::WAIT_CMD1: {
            cmd = (uint16_t)(byte) << 8;
            state = States::WAIT_CMD0;
        } break;
        case States::WAIT_CMD0: {
            cmd = cmd | (uint16_t)(byte);
            state = States::WAIT_DATA8;
        } break;
        case States::WAIT_DATA8: {
            k = byte;
            state = States::WAIT_DATA7;
        } break;
            // данные со счетчиков a b c d
        case States::WAIT_DATA7: {
            a = (uint16_t)(byte) << 8;
            state = States::WAIT_DATA6;
        } break;
        case States::WAIT_DATA6: {
            a = a | (uint16_t)(byte);
            state = States::WAIT_DATA5;
        } break;
        case States::WAIT_DATA5: {
            b = (uint16_t)(byte) << 8;
            state = States::WAIT_DATA4;
        } break;
        case States::WAIT_DATA4: {
            b = b | (uint16_t)(byte);
            state = States::WAIT_DATA3;
        } break;
        case States::WAIT_DATA3: {
            c = (uint16_t)(byte) << 8;
            state = States::WAIT_DATA2;
        } break;
        case States::WAIT_DATA2: {
            c = c | (uint16_t)(byte);
            state = States::WAIT_DATA1;
        } break;
        case States::WAIT_DATA1: {
            state = States::WAIT_DATA0;
            d = (uint16_t)(byte) << 8;
        } break;
        case States::WAIT_DATA0: {
            state = States::WAIT_CRC;
            d = d | (uint16_t)(byte);
        } break;
        // Проверяется контрольная сумма (XOR всех байтов). Если CRC совпадает
        case States::WAIT_CRC: {
            uint crc = cmd ^ a ^ b ^ c ^ d;
            if (crc == byte) {

                if (cmd==0x0002){
                    // При успешной обработке данных вызывается сигнал timeChanged,
                    // который можно использовать для обновления графического интерфейса.
                    qDebug() << "state CRC " << a << ":" << b << ":" << c << ":" << d;
                    emit this->koeffChanged(k);
                    emit this->dataChanged(a,b,c,d);
                }

            }
            // Возвращение в начало - ожидание приема байтов
            state = States::WAIT_START;
        } break;
        }
    }
}


