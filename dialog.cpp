#include "dialog.h"
#include "ui_dialog.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    isRefresh=false;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setSomeEnable(bool choice)
{
    if(choice==true){
        this->ui->lineEdit_3->setEnabled(true);
        this->ui->lineEdit_4->setEnabled(true);
        flag=2;
    }else if(choice==false){
        this->ui->lineEdit_3->setEnabled(false);
        this->ui->lineEdit_4->setEnabled(false);
        flag=1;
    }
}

void Dialog::setAtributes(QStringList* message,QStringList* user,QString instrumentCode)
{
    this->messageList=message;
    this->userList=user;
    this->instrumentCode=instrumentCode;
}

void Dialog::empty()
{
    this->ui->lineEdit->setText("");
    this->ui->lineEdit_2->setText("");
    this->ui->lineEdit_3->setText("");
    this->ui->lineEdit_4->setText("");
}

void Dialog::getIdByName(QString &name, QString &id)
{
    for (int i=0;i<userList->size();i++) {
        QStringList tempList=userList->at(i).split(" ");
        if(tempList.at(1)==name){
            id=tempList.at(0);
            break;
        }
    }
}

bool Dialog::writeFile(const QString &fileAddress, QStringList *list)
{
    QFile file(fileAddress);
    if(!file.open( QIODevice::WriteOnly|QIODevice::Text)){
        return false;
    }
    QTextStream stream(&file);
    stream<<list->join("\n")<<"\n";
    file.close();
    return true;
}

void Dialog::on_pushButton_2_clicked()
{
    if(this->ui->lineEdit->text().isEmpty()&&this->ui->lineEdit_2->text().isEmpty()
            &&this->ui->lineEdit_3->text().isEmpty()&&this->ui->lineEdit_4->text().isEmpty()){
        this->close();
    }else{
        QMessageBox w;
        w.setIcon(QMessageBox::Warning);
        w.setText("确认取消？");
        w.addButton("确定",QMessageBox::AcceptRole);
        w.addButton("取消",QMessageBox::RejectRole);
        int re=w.exec();
        if(re==QMessageBox::AcceptRole){
            this->close();
        }
    }
}

void Dialog::on_pushButton_clicked()
{
    QString idSend,nameSend,idGet,nameGet;
    QStringList changeInfo;
    QString fileAddress="message/message.txt";
    nameSend=this->ui->lineEdit->text();
    nameGet=this->ui->lineEdit_2->text();
    getIdByName(nameSend,idSend);
    getIdByName(nameGet,idGet);
    if(idSend==""||idGet==""){
        QMessageBox w;
        w.setText("姓名填写有误");
        w.setIcon(QMessageBox::Warning);
        w.exec();
    }else{
        if(this->flag==1){
            changeInfo.append("B");
            changeInfo.append(Global::getInstant()->getUserid());
            changeInfo.append(instrumentCode);
            changeInfo.append(idSend);
            changeInfo.append("-1");
            changeInfo.append(idGet);
            changeInfo.append("-1");
            isRefresh=true;
            messageList->append(changeInfo.join(" "));
            writeFile(fileAddress,messageList);
            emit refreshSignal();
            this->close();
        }else if(this->flag==2){
            changeInfo.append("C");
            changeInfo.append(Global::getInstant()->getUserid());
            changeInfo.append(instrumentCode);
            changeInfo.append("-1");
            changeInfo.append(idSend);
            changeInfo.append("-1");
            changeInfo.append(idGet);
            changeInfo.append("-1");
            isRefresh=true;
            messageList->append(changeInfo.join(" "));
            writeFile(fileAddress,messageList);
            emit refreshSignal();
            this->close();
        }
    }
}

void Dialog::setIsRefresh(bool value)
{
    isRefresh = value;
}

bool Dialog::getIsRefresh() const
{
    return isRefresh;
}
