#ifndef PARSERWGT_H
#define PARSERWGT_H

#include <QtWidgets/QWidget>
#include <QStringList>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include "comportreaderwriter.h"
#include <QtMath>

namespace Ui {
class ParserWgt;
}

class ParserWgt : public QWidget
{
    Q_OBJECT
public:
    explicit ParserWgt(QWidget *parent = 0);
    ~ParserWgt();

signals:
    //сигнал с принятыми данными
    void x4101_data(x4101_t data);
    void x4102_data(unsigned int data);
    void x4104_data(x4104_t data);
public slots:
    void get_x4101(x4101_t data);
    void get_x4102(unsigned int data);
    void get_x4104(x4104_t data);
protected slots:



private:
    Ui::ParserWgt *ui;
};

#endif // PARSERWGT_H
