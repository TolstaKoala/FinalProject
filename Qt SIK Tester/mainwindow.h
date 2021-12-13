#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "comportsettingswgt.h"
#include "comportreaderwriter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    //передает настройки компорта
    void com_port_settings_ready(com_port_setting_t comPortInfo);

    //состояние порта оподключено/отключено
    void port_open_state_ready(bool status);

    //сигнал с данными пакета
    void passportDataReady(QByteArray data);
    void x4101_data(x4101_t data);
    void x4102_data(unsigned int data);


    void ackNackStateReady(bool state);

public slots:
    //слот для отображения стутуса
    void parseCurStatus(QString str);


private slots:
    //


private:
    //в этот байт эррей собираем пакет об успешном приеме
    QByteArray curPacketByteArray;
    //этот байт эррей хранит пакет подвержения
    QByteArray ackByteArray;
    bool startPacket;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
