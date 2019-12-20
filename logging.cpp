#include "logging.h"
#include "ui_logging.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include "global.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
Logging::Logging(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logging)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    username="admin";
    password="123456";
    users=new QStringList;
    if(!readFile("baseinformation/user.txt",users)){
        QMessageBox::critical(this,"错误","系统文件丢失");
    }
}

Logging::~Logging()
{
    delete ui;
}

void Logging::on_pb_quit_clicked()
{
    this->close();
}

QString Logging::encode(QString &s)
{
    QByteArray ps;
    ps.append(s);
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(ps);
    QByteArray result=hash.result();
    QString md5=result.toHex();
    return md5;
}

bool Logging::readFile(QString path, QStringList *container)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        container->append(line);
    }
    return true;
}

void Logging::on_pb_ok_clicked()
{
    Global *temp=Global::getInstant();
    if(this->ui->comboBox->currentText()=="管理员"){
        if(this->ui->le_username->text()==username&&this->ui->le_password->text()==password){
            temp->setUsertype(this->ui->comboBox->currentIndex());
           this->accept();
        }
        else{
            QMessageBox::critical(this,"错误","密码或用户名错误！");
        }
    }else{
        username=this->ui->le_username->text();
        password=this->ui->le_password->text();
        password=encode(password);
        for(int i=0;i<users->length();i++){
            if(username==users->at(i).split(" ").at(0)){
                if(password==users->at(i).split(" ").at(4)&&this->ui->comboBox->currentIndex()>=users->at(i).split(" ").at(3).toInt()){
                    temp->setUsertype(this->ui->comboBox->currentIndex());
                    temp->setUserid(username);
                    this->accept();
                    return;
                }
                break;
            }
        }
        QMessageBox::critical(this,"错误","密码或用户名错误！");
    }
}

void Logging::on_le_username_returnPressed()
{
    if(this->ui->le_username->text()!="")
        this->ui->le_password->setFocus();
}

void Logging::on_le_password_returnPressed()
{
    if(this->ui->le_password->text()!="")
        this->ui->pb_ok->clicked();
}
