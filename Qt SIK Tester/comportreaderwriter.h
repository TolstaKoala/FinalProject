#ifndef COMPORTREADERWRITER_H
#define COMPORTREADERWRITER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include "comportsettingswgt.h"

#include "ringbuffer.h"

typedef struct{
    unsigned int lon;
    unsigned int lat;
    unsigned int att;
    unsigned int info;
    unsigned char min;
    unsigned char sec;
}x4101_t;

typedef struct{
    double x;
    double y;
    double z;
}x4104_t;

class ComPortReaderWriter : public QObject
{
    Q_OBJECT
public:
    explicit ComPortReaderWriter(QObject *parent = 0);
    ~ComPortReaderWriter();

signals:
    //состояние порта оподключено/отключено
    void port_open(bool state);

    void x4101_data(x4101_t data);
    void x4102_data(unsigned int data);
    void x4104_data(x4104_t data);
    //сигнал по приему данных
    void receiveData(QByteArray data);

public slots:
    //принимаем номер порта и бодрейт из гуи
    void parse_com_port_settings(com_port_setting_t com_port_settings);

    //слот на отправку данных
//    void sendData(QByteArray data);

protected slots:
    //слот обработки сигналов приема сообщений по ком порту
    void comPortDataReady();
    void send0x4101();
    void send0x4103();
    void send0x4104();
    void checkError();
protected:
    circular_buffer cb;
    QSerialPort* comPort;

    //настройки ком порта
    com_port_setting_t com_port_local_settings;
};

#endif // COMPORTREADERWRITER_H
