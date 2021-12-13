#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //настройки компорта из интерфейса в объект компорта
    QApplication::connect(ui->comPortSettingsWgt,SIGNAL(com_port_settings_ready(com_port_setting_t)),\
                          this, SIGNAL(com_port_settings_ready(com_port_setting_t)));
    //статус компорта из объекта компорта в интерфейс
    QApplication::connect(this, SIGNAL(port_open_state_ready(bool)), \
                          ui->comPortSettingsWgt,SLOT(parse_open_port_status(bool)));
//    //данные на отправку
//    connect(ui->passportWgt, SIGNAL(passportDataReady(QByteArray)), this, SIGNAL(passportDataReady(QByteArray)));

//    //тут подключаем данные, которые приняли к слоту, который парсит пакет с сигналом об успешном приеме
    connect(this, SIGNAL(x4101_data(x4101_t)), ui->parserWgt, SLOT(get_x4101(x4101_t)));

    connect(this, SIGNAL(x4102_data(unsigned int)),  ui->parserWgt, SLOT(get_x4102(unsigned int)));

//    //текущее состояние для отображения данных в строке состояния
//    connect(ui->passportWgt, SIGNAL(currentStateReady(QString)), this, SLOT(parseCurStatus(QString)));

//    //сигнал о получении прдтверждения, для выключения таймера ожидания подвтерждения
//    connect(this, SIGNAL(ackNackStateReady(bool)), ui->passportWgt, SLOT(parseAckNack(bool)));

    this->setWindowTitle(QString("SIK Test"));

    //начальное значение
    startPacket=false;
    //пакет отправки
    ackByteArray.append(0xCC);
    ackByteArray.append(0xBB);
    ackByteArray.append(0xAA);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
*   Парсим текущее состояние
*/
void MainWindow::parseCurStatus(QString str)
{
    ui->statusBar->showMessage(str);
}
