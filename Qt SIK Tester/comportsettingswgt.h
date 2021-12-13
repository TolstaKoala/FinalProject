#ifndef COMPORTSETTINGSWGT_H
#define COMPORTSETTINGSWGT_H

#include <QtWidgets/QWidget>
#include <QStringList>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>


#define COM_PORT_CHECK_INTERVAL 1000


typedef struct
{
    QString portName;
    quint32 portBoudrate;
} com_port_setting_t;


Q_DECLARE_METATYPE(com_port_setting_t)

namespace Ui {
class ComPortSettingsWgt;
//Q_DECLARE_METATYPE(com_port_setting_t)

}

class ComPortSettingsWgt : public QWidget
{
    Q_OBJECT

public:
    explicit ComPortSettingsWgt(QWidget *parent = 0);
    ~ComPortSettingsWgt();

signals:
    //передает настройки компорта
    void com_port_settings_ready(com_port_setting_t comPortInfo);

public slots:
    //парсим текущее состояние порта открыт/закрыт
    void parse_open_port_status(bool status);

protected slots:
    //функция проверяет периодически доступность компортов в системе и отправляет сигнал, о доступном списке
    void check_system_comports();

    //сохраняет имя выбранного компорта
    void save_com_port_change(QString comPortName);

    //применяет выбранные настройки к выбранному компорту по нажатию кнопки
    void configure_com_port();

protected:
    //устанавливает достпуные бодрейты в интерфейсе
    void set_available_boudrates();

    QString selectedPortName;

private:
    Ui::ComPortSettingsWgt *ui;
};

#endif // COMPORTSETTINGSWGT_H
