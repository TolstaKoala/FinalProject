#include "mainwindow.h"
#include <QApplication>
#include <comportreaderwriter.h>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ComPortReaderWriter comPortReaderWriter;

    QThread comPortReaderWriterThread;

    //Передаем настройки ком порта из интерфейса
    QApplication::connect(&w, SIGNAL(com_port_settings_ready(com_port_setting_t)),&comPortReaderWriter, SLOT(parse_com_port_settings(com_port_setting_t)));
    //Передаем статус компорта в интерфейс
    QApplication::connect(&comPortReaderWriter, SIGNAL(port_open(bool)), &w, SIGNAL(port_open_state_ready(bool)));
    //Передаем данные из интерфейса
    //QApplication::connect(&w, SIGNAL(passportDataReady(QByteArray)), &comPortReaderWriter, SLOT(sendData(QByteArray)));
    //передаем данные с компорта
    QApplication::connect(&comPortReaderWriter, SIGNAL(x4101_data(x4101_t)), &w, SIGNAL(x4101_data(x4101_t)));
    QApplication::connect(&comPortReaderWriter, SIGNAL(x4102_data(unsigned int)), &w, SIGNAL(x4102_data(unsigned int)));
    QApplication::connect(&comPortReaderWriter, SIGNAL(x4104_data(unsigned int)), &w, SIGNAL(x4104_data(x4104_t)));


    comPortReaderWriter.moveToThread(&comPortReaderWriterThread);
    comPortReaderWriterThread.start(QThread::HighPriority);

    w.show();

    return a.exec();
}
