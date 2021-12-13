#include "comportsettingswgt.h"
#include "ui_comportsettingswgt.h"

ComPortSettingsWgt::ComPortSettingsWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComPortSettingsWgt)
{
    ui->setupUi(this);

    QTimer* comPortCheckTimer=new QTimer(this);
    comPortCheckTimer->setInterval(COM_PORT_CHECK_INTERVAL);

    connect(comPortCheckTimer, SIGNAL(timeout()), this, SLOT(check_system_comports()));

    set_available_boudrates();

    selectedPortName.clear();
    ui->comPort_comboBox->setInsertPolicy(QComboBox::InsertAtTop);

    connect(ui->comPort_comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(save_com_port_change(QString)));

    connect(ui->saveSettings_pushButton, SIGNAL(clicked()), this, SLOT(configure_com_port()));

    comPortCheckTimer->start();


    qRegisterMetaType<com_port_setting_t>("com_port_setting_t");
}

ComPortSettingsWgt::~ComPortSettingsWgt()
{
    delete ui;
}


/*
*  Функция периодически опрашивает компорты на их наличие в системе, по таймеру
*/
void ComPortSettingsWgt::check_system_comports()
{
    QStringList portList, portListNew;

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
//        qDebug()<<"Name"<<info.portName();
        portList.append(info.portName());
    }

    ui->comPort_comboBox->clear();

    if(!selectedPortName.isEmpty())
    {
        portList.removeOne(selectedPortName);
        portListNew<<selectedPortName;
        portListNew=portListNew+portList;
    }
    else
    {
        portListNew=portList;
    }

    ui->comPort_comboBox->addItems(portListNew);
}


/*
*  Функция заполняет список доступных бодрейтов в интерфейсе
*/
void ComPortSettingsWgt::set_available_boudrates()
{
    QStringList boudrateList;
    boudrateList<<"9600"<<"38400"<<"115200";

    ui->baudRate_comboBox->addItems(boudrateList);
}

/*
*   Функция сохраняет выбранный в комбобоксе номер компорта
*/
void ComPortSettingsWgt::save_com_port_change(QString comPortName)
{
    if(comPortName.isEmpty())
        return;
//    qDebug()<<"Selected Com Port Name"<<comPortName;
    selectedPortName=comPortName;
}


/*
 *  Обрабатывает нажатие на кнопку применения настроек и генерирует сигнал с бодрейтом и номером порта
*/
void ComPortSettingsWgt::configure_com_port()
{
    com_port_setting_t com_port_setting;

    com_port_setting.portName=selectedPortName;
    com_port_setting.portBoudrate=ui->baudRate_comboBox->currentText().toUInt();

    qDebug()<<"Current Saved Port is"<<com_port_setting.portName;
    qDebug()<<"Current Saved Baudrate is"<<com_port_setting.portBoudrate;

    emit com_port_settings_ready(com_port_setting);
}

void ComPortSettingsWgt::parse_open_port_status(bool status)
{
    if(status==false)
    {
        ui->current_state_label->setText(tr("Порт %1 закрыт").arg(selectedPortName));
    }
    else
    {
       ui->current_state_label->setText(tr("Порт %1 открыт").arg(selectedPortName));
    }
}
