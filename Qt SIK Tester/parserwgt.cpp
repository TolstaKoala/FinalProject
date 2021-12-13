#include "parserwgt.h"
#include "ui_parserwgt.h"

ParserWgt::ParserWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParserWgt)
{
    ui->setupUi(this);

    connect(this, SIGNAL(x4101_data(x4101_t)), this, SLOT(get_x4101(x4101_t)));

    connect(this, SIGNAL(x4102_data(unsigned int)), this, SLOT(get_x4102(unsigned int)));
    ui->count1->setDecMode();
    ui->count1->setPalette(Qt::red);
    //ui->count2->setDecMode();
    //ui->count2->setPalette(Qt::blue);

    connect(this, SIGNAL(x4104_data(unsigned int)), this, SLOT(get_x4104(unsigned int)));
}


ParserWgt::~ParserWgt()
{
    delete ui;
}

void ParserWgt::get_x4104(x4104_t data){
    QString x_str, y_str,z_str;
    double x, y, z;
    x = data.x;
    y = data.y;
    z = data.z;
    x_str.append(QString::number(x)+" m");
    y_str.append(QString::number(y)+" m");
    z_str.append(QString::number(z)+" m");
    ui->x_l->setText(x_str);
    ui->y_l->setText(y_str);
    ui->z_l->setText(z_str);
}

double counter2 = 0.0;
void ParserWgt::get_x4102(unsigned int data){
    QString strSpeed;
    float speed;
    speed = (float)data*36/100;
    strSpeed.append(QString::number(speed)+" km/h");
    counter2 = counter2 + 1.0;
    ui->speed->setText(strSpeed);
    //ui->count2->display(counter2);
}

double counter1 = 0.0;

void ParserWgt::get_x4101(x4101_t data)
{
    QString strLat, strLon, strAtt, strTime;
    double lat, lon, att, f1, f2;
    strLat.clear();
    strLon.clear();
    strAtt.clear();

    counter1 = counter1 + 1.0;

    lat = data.lat*1E-8 * 180 / M_PI;
    f1 = (lat-floor(lat))*60;

    lon = data.lon*1E-8* 180 / M_PI;
    f2 = (lon-floor(lon))*60;

    att = (double)data.att/100;

    strLat.append(QString::number(floor(lat))+"°"+QString::number(floor(f1))+","+QString::number(floor(1000* (f1-floor(f1)) ))+"'");
    strLon.append(QString::number(floor(lon))+"°"+QString::number(floor(f2))+","+QString::number(floor(1000* (f2-floor(f2)) ))+"'");
    strAtt.append(QString::number(att));
    strTime.append(QString::number(data.min)+":"+QString::number(data.sec));
    ui->lat->setText(strLat);
    ui->lon->setText(strLon);
    ui->att->setText(strAtt);
    if(data.info)ui->info->setText("актуальная");
    else ui->info->setText("НЕактуальная");
    ui->time->setText(strTime);
    ui->count1->display(counter1);
}
