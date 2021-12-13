#include "comportreaderwriter.h"

#include "ringbuffer.h"
#include "parserstatistcs.h"

ComPortReaderWriter::ComPortReaderWriter(QObject *parent) :
    QObject(parent)
{
    comPort=new QSerialPort(this);
    connect(comPort, SIGNAL(readyRead()), this, SLOT(comPortDataReady()));

  //  connect(comPort, SIGNAL(errorOccurred()), this, SLOT(checkError()));

    QTimer* comPortSendTimer=new QTimer(this);
    comPortSendTimer->setInterval(COM_PORT_CHECK_INTERVAL);
    connect(comPortSendTimer, SIGNAL(timeout()), this, SLOT(send0x4101()));
    comPortSendTimer->start(1000);

    qRegisterMetaType<x4101_t>("x4101_t");
}

ComPortReaderWriter::~ComPortReaderWriter()
{
    if(comPort->isOpen())
    {
       comPort->close();
       emit port_open(false);
    }
}

/*
*   Принимаем и сохраняем номер порта и бодрейт
*/
void ComPortReaderWriter::parse_com_port_settings(com_port_setting_t com_port_settings)
{
    if(comPort->isOpen()){
        qDebug()<<"Закрываем ком порт";
        comPort->close();
    }

    com_port_local_settings=com_port_settings;
//    comPort->setFlowControl()
    comPort->setPortName(com_port_local_settings.portName);


    if(!(comPort->open(QIODevice::ReadWrite)))
    {
        qDebug()<<"Не могу открыть ком порт!";
        emit port_open(false);
    }

    else
    {
        comPort->setParity(QSerialPort::NoParity);
        comPort->setDataBits(QSerialPort::Data8);
        comPort->setStopBits(QSerialPort::OneStop);
        comPort->setFlowControl(QSerialPort::NoFlowControl);
        comPort->setBaudRate(com_port_local_settings.portBoudrate);


        qDebug()<<"Port "<<com_port_local_settings.portName<<" open with baudrate "<<com_port_local_settings.portBoudrate;
        emit port_open(true);

     }
}

 void ComPortReaderWriter::checkError(){
     int i = 1;
 }

/*
*   Отправка данных по ком порту
*/
 void ComPortReaderWriter::send0x4101()
 {
     if(comPort->isOpen()){
    QByteArray data;
    data.append(0x41);

     comPort->setParity(QSerialPort::MarkParity);
     if(!comPort->write(data))
         qDebug()<<"Ошибка отправки пакета";
     else
         comPort->waitForBytesWritten(-1);
     data.clear();
     data.append(0x01);
     comPort->setParity(QSerialPort::SpaceParity);
     if(!comPort->write(data))
         qDebug()<<"Ошибка отправки пакета";
     else
         comPort->waitForBytesWritten(-1);
    }
     qDebug()<<"Send 4101";
}

 void ComPortReaderWriter::send0x4103()
 {
     if(comPort->isOpen()){
     QByteArray data;
     data.append(0x41);

      comPort->setParity(QSerialPort::MarkParity);
      if(!comPort->write(data))
          qDebug()<<"Ошибка отправки пакета";
      else
          comPort->waitForBytesWritten(-1);
      data.clear();
      data.append(0x03);
      comPort->setParity(QSerialPort::SpaceParity);
      if(!comPort->write(data))
          qDebug()<<"Ошибка отправки пакета";
      else
          comPort->waitForBytesWritten(-1);
    }
     qDebug()<<"Send 4103";
}

 void ComPortReaderWriter::send0x4104()
 {
     if(comPort->isOpen()){
     QByteArray data;
     data.append(0x41);

      comPort->setParity(QSerialPort::MarkParity);
      if(!comPort->write(data))
          qDebug()<<"Ошибка отправки пакета";
      else
          comPort->waitForBytesWritten(-1);
      data.clear();
      data.append(0x04);
      comPort->setParity(QSerialPort::SpaceParity);
      if(!comPort->write(data))
          qDebug()<<"Ошибка отправки пакета";
      else
          comPort->waitForBytesWritten(-1);
    }
     qDebug()<<"Send 4102";
}

 /*
 *   Принимает данные из com порта
 */
QByteArray wholePacket;
int pack = 0;
 void ComPortReaderWriter::comPortDataReady()
 {
     QByteArray dataR = comPort->readAll();
     qDebug()<<"Data is:"<<dataR.toHex();
     wholePacket.append(dataR);
     if(wholePacket.at(0)==0x41){
         int size = wholePacket.size();
         if(size == 16){

            /*Parse dataR*/
            x4101_t dataParsed;
            unsigned int tempo[15];
            for(int i = 0; i <15; i++){
                tempo[i] = (wholePacket.at(i+1))&0xff;
            }
            dataParsed.lat = tempo[0] + (tempo[1]*0x100) + (tempo[2]*0x10000)+ (tempo[3]*0x1000000);
            dataParsed.lon = tempo[4] + (tempo[5]<<8) + (tempo[6]<<16)+ (tempo[7]<<24);
            dataParsed.att = tempo[10] + (tempo[11]<<8) + (tempo[12]<<16)+ (tempo[13]<<24);
            dataParsed.info = tempo[14];
            dataParsed.min = tempo[9];
            dataParsed.sec = tempo[8];
            wholePacket.clear();
            pack = 1;
            emit x4101_data(dataParsed);
            /* Ask speed*/
            send0x4103();
        }else if (wholePacket.size() == 2 && pack){
            /* send speed */
             pack = 0;
             unsigned int tempo[4];
             for(int i = 0; i <1; i++){
                 tempo[i] = (wholePacket.at(i+1))&0xff;
             }
            unsigned int dataParsed = (unsigned int) tempo[0] ;
            wholePacket.clear();
            emit x4102_data(dataParsed);
            send0x4104();
        }else if (wholePacket.size() == 24 && !pack){
            /* send speed */
            pack = 0;
            unsigned int tempo[23];
            x4104_t dataParsed;
            for(int i = 0; i <23; i++){
                tempo[i] = (wholePacket.at(i+1))&0xff;
            }
            dataParsed.x = tempo[0] + (tempo[1]<<8) + (tempo[2]<<16)+ (tempo[3]<<24);
            dataParsed.y = tempo[4] + (tempo[5]<<8) + (tempo[6]<<16)+ (tempo[7]<<24);
            dataParsed.z = tempo[4] + (tempo[5]<<8) + (tempo[6]<<16)+ (tempo[7]<<24);
            wholePacket.clear();
            emit x4104_data(dataParsed);
        }
     }
//     }else {
         wholePacket.clear();
//     }

 }
