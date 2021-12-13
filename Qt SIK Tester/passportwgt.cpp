#include "passportwgt.h"
#include "ui_passportwgt.h"
#include <QDebug>

PassportWgt::PassportWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PassportWgt)
{
    ui->setupUi(this);


    //устанавливаем валидаторы на поля ввода
    //Кода производителя 4 заглавные буквы
    QRegExp kodPrStrValidator;
    kodPrStrValidator.setPattern("[А-Я]{4}");
    QRegExpValidator *validatorForKodPr = new QRegExpValidator(kodPrStrValidator, this);
    ui->kodPrlineEdit->setValidator(validatorForKodPr);
    ui->kodPrlineEdit->setText("АБВГ");

    //код устройства фиксированной длины формата xxxxxx.xxx
    QRegExp kodUstrStrValidator;
    kodUstrStrValidator.setPattern("[0-9]{6}[.]{1}[0-9]{3}");
    QRegExpValidator *validatorForKodUstr=new QRegExpValidator(kodUstrStrValidator, this);
    ui->kodUstrlineEdit->setValidator(validatorForKodUstr);
    ui->kodUstrlineEdit->setText("000000.000");

    //номер исполнения, число 16 бита, на усмотрение разработчика, макс 100
    QValidator *nomerIspValidator = new QIntValidator(0, 0x63, this);
    ui->NomerIsplineEdit->setValidator(nomerIspValidator);
    ui->NomerIsplineEdit->setText("0");

    //доп номер исполнения, число 16 бита, на усмотрение разработчика, макс 100
    QValidator *dopNomerIspValidator = new QIntValidator(0, 0x63, this);
    ui->dopNomerIsplineEdit->setValidator(dopNomerIspValidator);
    ui->dopNomerIsplineEdit->setText("0");

    //заводской номер 32 бита
    QValidator *zavNomerIspValidator = new QIntValidator(0, 0xFFFFFF, this);
    ui->zavNomlineEdit->setValidator(zavNomerIspValidator);
    ui->zavNomlineEdit->setText("0");

    //дополнительное обозначение 10 символов
    QRegExp dopObStrValidator;
    dopObStrValidator.setPattern("[0-9,А-Я,-]{16}");
    QRegExpValidator *validatorForDopObStr=new QRegExpValidator(dopObStrValidator, this);
    ui->dopOblineEdit->setValidator(validatorForDopObStr);

    //заводской номер, буквенно-цифровой
    ui->zavNomStrlineEdit->setValidator(validatorForDopObStr);

    //дата изготовления
    ui->dataIzgotdateTimeEdit->setMinimumDate(QDate(2010,1,1));

    //тип процессорного устройства
    QStringList *typeProcList;
    typeProcList=new QStringList();
    *typeProcList<<QString("0-Не определен")<<QString("1-Тест начального включения")<<QString("2-Начальный загрузчик")\
                   <<QString("3-Основная программа")<<QString("4-Доп программа")<<QString("5-ПЛИС ЦОС")\
                     <<QString("6-ПЛИС контр")<<QString("7-ПЛИС ЦКПИ");

    ui->comboBox->addItems(*typeProcList);

    //частота 0- не определена, 16 бит максимальный размер
    QValidator *chastProcValidator = new QIntValidator(0, 0xFFFF, this);
    ui->chastProclineEdit->setValidator(chastProcValidator);
    ui->chastProclineEdit->setText("0");

    //дата компиляции
    ui->dataFiledateTimeEdit->setMinimumDate(QDate(2010,1,1));

    //Версия ПО 32 бита
    QValidator *versPoValidator = new QIntValidator(0, 0xFFFFFF, this);
    ui->versPolineEdit->setValidator(versPoValidator);
    ui->versPolineEdit->setText("0");

    //Контрольная сумма
    QValidator *crcValidator = new QIntValidator(0, 0xFFFFFF, this);
    ui->crclineEdit->setValidator(crcValidator);
    ui->crclineEdit->setText("0");

    //Ревизия ЦПУ
    QValidator *revProcValidator = new QIntValidator(0, 0xFFFFFF, this);
    ui->revProclineEdit->setValidator(revProcValidator);
    ui->revProclineEdit->setText("0");

    //Дата компиляции ПО
    ui->dataCompdateTimeEdit->setMinimumDate(QDate(2010,1,1));

    codec = QTextCodec::codecForName("CP866");

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(parsePassportData()));

    ackWaitTimer=new QTimer(this);
    ackWaitTimer->setSingleShot(true);
    ackWaitTimer->setInterval(5000);

    connect(ackWaitTimer, SIGNAL(timeout()), this, SLOT(parseTimerEvent()));
}

PassportWgt::~PassportWgt()
{
    delete ui;
}

/*
*   Слот, в котором парсим данные папортов и собираем данные в пакет
*/
void PassportWgt::parsePassportData()
{
    QByteArray kodProizvByteArray;
    quint32 kodUstrInt;
    quint16 nomerIsp;
    quint16 dopNomerIsp;
    quint32 zavNomer;

    quint8 dopOboznSize; //размер реальной строки дополнительного обозначения
    QByteArray dopOboznByteArray;
    quint8 zavNomerStrSize; //размер реальной строки доп обозначения
    QByteArray zavNomerStrByteArray;
    date_time_u data_izg;
    ident_u     ident;
    date_time_u data_file;
    quint32 versPo;
    quint32 kontrSum;
    quint32 revProc;
    date_time_u data_compil;

    data.clear();

    //код производителя
    if(ui->kodPrlineEdit->text().isEmpty()){
        qDebug()<<"Не заполнено поле кода производителя";
    }
    else if(ui->kodPrlineEdit->text().size()!=4){
        qDebug()<<"Неверный размер кода производителя";
    }
    else{
//        QString kodProizv=ui->kodPrlineEdit->text();
        //конвертируем строку в CP866
        kodProizvByteArray=codec->fromUnicode(ui->kodPrlineEdit->text());
        qDebug()<<"Код производителя: "<<kodProizvByteArray.toHex();
    }

    //код устройства
    if(ui->kodUstrlineEdit->text().size()!=10){
        qDebug()<<"Неверный код устройства";
    }
    else{
        //удалим точку
        QString kodUstr=ui->kodUstrlineEdit->text();
        kodUstr.remove(QString("."), Qt::CaseSensitive);
        kodUstrInt=kodUstr.toUInt();

        qDebug()<<"Код устройства: "<<kodUstrInt;
    }

    //номер исполнения
    if(ui->NomerIsplineEdit->text().isEmpty()){
        qDebug()<<"Не заполнено поле номер исполнения";
    }
    else{
        nomerIsp=ui->NomerIsplineEdit->text().toUShort();
        qDebug()<<"Номер исполнения: "<<nomerIsp;
    }

    //дополнительный номер исполнения
    if(ui->dopNomerIsplineEdit->text().isEmpty()){
        qDebug()<<"Не заполнено поле доп номер исполнения";
    }
    else{
        dopNomerIsp=ui->dopNomerIsplineEdit->text().toUShort();
        qDebug()<<"Дополнительный номер исполнения: "<<dopNomerIsp;
    }

    //заводской номер
    if(ui->zavNomlineEdit->text().isEmpty()){
        qDebug()<<"Заводской номер не введен";
    }
    else{
        zavNomer=ui->zavNomlineEdit->text().toUInt();
        qDebug()<<"Заводской номер:"<<zavNomer;
    }

    //дополнительное обозначение
    if(ui->dopOblineEdit->text().isEmpty()){
        qDebug()<<"Дополнительное обозначени не введено";
        dopOboznSize=0;
        while(dopOboznByteArray.size()!=STR_MAX_SIZE)
            dopOboznByteArray.append(0xFF);
    }
    else{
        dopOboznByteArray=codec->fromUnicode(ui->dopOblineEdit->text());
        dopOboznSize=dopOboznByteArray.size();
        while(dopOboznByteArray.size()!=STR_MAX_SIZE)
            dopOboznByteArray.append(0xFF);
        qDebug()<<"Дополнительное обозначение: "<<dopOboznByteArray.toHex();
    }

    //заводской номер строкой
    if(ui->zavNomStrlineEdit->text().isEmpty()){
        qDebug()<<"Заводской строковый номер не указан";
        zavNomerStrSize=0;
        while(zavNomerStrByteArray.size()!=STR_MAX_SIZE)
             zavNomerStrByteArray.append(0xFF);
    }
    else{
        zavNomerStrByteArray=codec->fromUnicode(ui->zavNomStrlineEdit->text());
        zavNomerStrSize=zavNomerStrByteArray.size();
        while(zavNomerStrByteArray.size()!=STR_MAX_SIZE)
             zavNomerStrByteArray.append(0xFF);
    }

    //заполняем дату изготовления
    data_izg.array=0;
    data_izg.date_time.sec=ui->dataIzgotdateTimeEdit->dateTime().time().second();
    data_izg.date_time.min=ui->dataIzgotdateTimeEdit->dateTime().time().minute();
    data_izg.date_time.chas=ui->dataIzgotdateTimeEdit->dateTime().time().hour();

    data_izg.date_time.god=ui->dataIzgotdateTimeEdit->dateTime().date().year()-2010; //год задается относительно 2010 года
    if((ui->dataIzgotdateTimeEdit->dateTime().date().year()-2010)<0)
        qDebug()<<"Неверно введен год, считается от 2010";
    data_izg.date_time.mes=ui->dataIzgotdateTimeEdit->dateTime().date().month();
    data_izg.date_time.den=ui->dataIzgotdateTimeEdit->dateTime().date().day();

    qDebug()<<"Дата изготовления: день"<<data_izg.date_time.den<<" месяц "<<data_izg.date_time.mes<<" год "<<data_izg.date_time.god+2010\
              <<" часы "<<data_izg.date_time.chas<<" минуты "<<data_izg.date_time.min<<" секунды "<<data_izg.date_time.sec;

    //заполняем общую информацию
    ident.array=0;
    //тип
    ident.ident.tip=ui->comboBox->currentIndex();

    if(ui->chastProclineEdit->text().isEmpty()){
       qDebug()<<"Не задана частота процессора";
    }
    else{
       ident.ident.chast=ui->chastProclineEdit->text().toUShort();
       qDebug()<<"Частота процессора"<<ident.ident.chast;
    }

    //дата файла
    data_file.array=0;
    data_file.date_time.sec=ui->dataFiledateTimeEdit->dateTime().time().second();
    data_file.date_time.min=ui->dataFiledateTimeEdit->dateTime().time().minute();
    data_file.date_time.chas=ui->dataFiledateTimeEdit->dateTime().time().hour();

    data_file.date_time.god=ui->dataFiledateTimeEdit->dateTime().date().year()-2010; //год задается относительно 2010 года
    if((ui->dataFiledateTimeEdit->dateTime().date().year()-2010)<0)
        qDebug()<<"Неверно введен год для даты создания файла, считается от 2010";
    data_file.date_time.mes=ui->dataFiledateTimeEdit->dateTime().date().month();
    data_file.date_time.den=ui->dataFiledateTimeEdit->dateTime().date().day();

    qDebug()<<"Дата файла: день"<<data_file.date_time.den<<" месяц "<<data_file.date_time.mes<<" год "<<data_file.date_time.god+2010\
              <<" часы "<<data_file.date_time.chas<<" минуты "<<data_file.date_time.min<<" секунды "<<data_file.date_time.sec;

    //версия ПО
    if(ui->versPolineEdit->text().isEmpty()){
        qDebug()<<"Пустое поле версии ПО";
        emit currentStateReady(QString("Пустое поле версии ПО"));
    }
    else{
        versPo=ui->versPolineEdit->text().toUInt();
        qDebug()<<"Версия ПО"<<versPo;
    }

    //контрольная сумма
    if(ui->crclineEdit->text().isEmpty()){
        qDebug()<<"Контрольная сумма ПО не введена";
        emit currentStateReady(QString("Контрольная сумма ПО не введена"));
    }
    else{
        kontrSum=ui->crclineEdit->text().toUInt();
        qDebug()<<"Контрольная сумма: "<<kontrSum;
    }

    //ревизия процессора
    if(ui->revProclineEdit->text().isEmpty()){
        qDebug()<<"Ревизия процессора не введена";
        emit currentStateReady(QString("Ревизия процессора не введена"));
    }
    else{
       revProc=ui->revProclineEdit->text().toUInt();
       qDebug()<<"Ревизия процессора:"<<revProc;
    }

    //дата компиляции
    data_compil.array=0;
    data_compil.array=0;
    data_compil.date_time.sec=ui->dataCompdateTimeEdit->dateTime().time().second();
    data_compil.date_time.min=ui->dataCompdateTimeEdit->dateTime().time().minute();
    data_compil.date_time.chas=ui->dataCompdateTimeEdit->dateTime().time().hour();

    data_compil.date_time.god=ui->dataCompdateTimeEdit->dateTime().date().year()-2010; //год задается относительно 2010 года
    if((ui->dataCompdateTimeEdit->dateTime().date().year()-2010)<0)
        qDebug()<<"Неверно введен год для даты создания файла, считается от 2010";
    data_compil.date_time.mes=ui->dataCompdateTimeEdit->dateTime().date().month();
    data_compil.date_time.den=ui->dataCompdateTimeEdit->dateTime().date().day();

    qDebug()<<"Дата компиляции: день"<<data_compil.date_time.den<<" месяц "<<data_compil.date_time.mes<<" год "<<data_compil.date_time.god+2010\
              <<" часы "<<data_compil.date_time.chas<<" минуты "<<data_compil.date_time.min<<" секунды "<<data_compil.date_time.sec;

    //собираем пакет
    //пишем id
    data.append(0xAA); //1
    data.append(0xBB); //1
    data.append(0xCC); //1
    data.append(0xDD); //1


    //код заголовка
    //0x1018, фикисированный размер пакета
    data.append(kodProizvByteArray); //4

    QByteArray data1018ByteArray;
    data1018ByteArray.resize(sizeof(kodUstrInt)+sizeof(nomerIsp)+sizeof(dopNomerIsp)+sizeof(zavNomer));
//    data.append(kodUstrByteArray);   //4
//    data.append(nomerIsp);           //2
//    data.append(dopNomerIsp);        //2
//    data.append(zavNomer);           //4

    memcpy(data1018ByteArray.data(), &kodUstrInt, sizeof(kodUstrInt));
    memcpy(data1018ByteArray.data()+sizeof(kodUstrInt), &nomerIsp, sizeof(nomerIsp));
    memcpy(data1018ByteArray.data()+sizeof(kodUstrInt)+sizeof(nomerIsp), &dopNomerIsp, sizeof(dopNomerIsp));
    memcpy(data1018ByteArray.data()+sizeof(kodUstrInt)+sizeof(nomerIsp)+ sizeof(dopNomerIsp), &zavNomer, sizeof(zavNomer));

    data.append(data1018ByteArray);

    //0x1030
    data.append(dopOboznSize);          //1 размер полезных данных
    data.append(dopOboznByteArray);     //16 тут размер можно увеличить
    data.append(zavNomerStrSize);       //1 размер
    data.append(zavNomerStrByteArray);  //16 тут размер можно увеличить

    QByteArray dataIzgByteArray;
    dataIzgByteArray.resize(4);
    memcpy(dataIzgByteArray.data(), &data_izg.array, 4);
    //data.append(data_izg.array);        //4
    data.append(dataIzgByteArray);

    //0x107B
    QByteArray data107BByteArray;
    quint8 data1017shift=0;
    data107BByteArray.resize(sizeof(ident.array)+sizeof(data_file.array)+sizeof(versPo)+sizeof(kontrSum)+sizeof(revProc)+sizeof(data_compil.array));

    memcpy(data107BByteArray.data(), &ident.array, sizeof(ident.array));
    data1017shift+= sizeof(ident.array);
    memcpy(data107BByteArray.data()+data1017shift, &data_file.array, sizeof(data_file.array));
    data1017shift+=sizeof(data_file.array);
    memcpy(data107BByteArray.data()+data1017shift, &versPo, sizeof(versPo));
    data1017shift+=sizeof(versPo);
    memcpy(data107BByteArray.data()+data1017shift, &kontrSum, sizeof(kontrSum));
    data1017shift+=sizeof(kontrSum);
    memcpy(data107BByteArray.data()+data1017shift, &revProc, sizeof(revProc));
    data1017shift+=sizeof(revProc);
    memcpy(data107BByteArray.data()+data1017shift, &data_compil.array, sizeof(data_compil.array));

    data.append(data107BByteArray);

//    data.append(ident.array);           //4
//    data.append(data_file.array);       //4
//    data.append(versPo);                //4
//    data.append(kontrSum);              //4
//    data.append(revProc);               //4
//    data.append(data_compil.array);     //4

    //добавить crc 16
    QByteArray crc16ByteArray;
    quint8 *startBlock;
    quint8 dataSize;
    startBlock=(quint8*)data.data()+4; //4 байта заголовок
    dataSize=data.size();
    quint16 crc16=crc16ccitt(startBlock,dataSize);

    crc16ByteArray.resize(2);
    memcpy(crc16ByteArray.data(), &crc16, 2);
    data.append(crc16ByteArray);                 //2
    qDebug()<<"crc"<<crc16;

    qDebug()<<"Массив для отправки:"<<data.toHex();

    //можно отправлять!
    emit currentStateReady(QString("Отправляем данные"));
    emit passportDataReady(data);

    ackWaitTimer->start();
}


/*
*  Функция подсчета CRC
*/
quint16 PassportWgt::crc16ccitt(quint8 *block,quint32 blockLength)
{
    quint16 crc=0;
    while(blockLength--){
        quint16 tmp = (crc >> 8) ^ (quint16) *block++;
        crc = (crc << 8) ^ crc16_ccitt_table[tmp];
    }
    return crc;
}

/*
*  Функция обработки таймера
*/
void PassportWgt::parseTimerEvent()
{
     emit currentStateReady(QString("Таймаут передачи данных"));
}

/*
*   Если данные успешно записаны, сбрасываем таймер ожидания
*/
void PassportWgt::parseAckNack(bool state)
{
    if(state)
       ackWaitTimer->stop();
}
