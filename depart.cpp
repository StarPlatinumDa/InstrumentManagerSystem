#include "depart.h"
#include "ui_depart.h"
#include<QPainter>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<QFont>
#include"global.h"
#include<QDebug>
depart::depart(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::depart)
{
    //this->setStyleSheet("background-image:/new/prefix1/picture/background.jpg");
    ui->setupUi(this);
    Global *temp=Global::getInstant();
    allapply=new QStringList;//读取申请个数
    readfile("message/message.txt",allapply);
    thisapply=new QStringList;
    for(int i=0;i<allapply->length();i++){
        if(temp->getInstrumentName()==finddepartbyid(allapply->at(i).split(" ").at(1))){
            thisapply->append(allapply->at(i));
        }
    }
    QFont ft;
    ft.setBold(true);//加粗
    ft.setPointSize(24);//21号字体
    ui->label_5->setText(QString::number(thisapply->length()));
    dep=new QStringList;
    readfile("baseinformation/department.txt",dep);
    for(int i=0;i<dep->length();i++){
        if(dep->at(i).split(" ").at(0)==temp->getInstrumentName()){
            //ui->label_2->setFont(ft);
            ui->label_2->setText(dep->at(i).split(" ").at(1));
            break;
        }
    }
}

depart::~depart()
{
    delete ui;
}

void depart::readfile(QString filename, QStringList *str)
{
    QFile f1(filename);
    f1.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream stream1(&f1);
    while(!stream1.atEnd())
    {
        str->append(stream1.readLine());
    }
    f1.close();
}

QString depart::finddepartbyid(QString id)
{
    QStringList *str=new QStringList;
    readfile("baseinformation/user.txt",str);
    for(int i=0;i<str->length();i++){
        if(id==str->at(i).split(" ").at(0)){
            return str->at(i).split(" ").at(2);
        }
    }
    return "";
}

void depart::on_pb_enter_clicked()
{
    this->close();
    Sub1 w;
    w.exec();
}
