#include "addinstrument.h"
#include "ui_addinstrument.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDateTime>
#include <QMessageBox>
#include <QDateTime>
#include <global.h>
#include <QtDebug>
#include "hander.h"
AddInstrument::AddInstrument(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddInstrument)
{
    ui->setupUi(this);
}

AddInstrument::~AddInstrument()
{
    delete ui;
}

void AddInstrument::on_pushButton_2_clicked()
{
    this->close();
}

void AddInstrument::on_pushButton_clicked()
{
    QString name=this->ui->le_name->text();
    QString mode=this->ui->le_nodel->text();
    QString number=this->ui->le_number->text();

}

void AddInstrument::on_le_name_returnPressed()
{
    this->ui->le_number->setFocus();
}

void AddInstrument::on_le_number_returnPressed()
{
    this->ui->le_nodel->setFocus();
}

void AddInstrument::on_pb_cancel_clicked()
{
    if((this->ui->le_name->text()!="")||(this->ui->le_nodel->text()!="")||(this->ui->le_number->text()!="")){
        QMessageBox w;
        w.setText("确定放弃？");
        w.setWindowTitle("提示");
        w.addButton("确定",QMessageBox::AcceptRole);
        w.addButton("取消",QMessageBox::RejectRole);
        w.setIcon(QMessageBox::Warning);
        int re=w.exec();
        if(re==QMessageBox::AcceptRole){
            this->close();
        }
    }else{
        this->close();
    }

}
void AddInstrument::on_pb_ok_clicked()
{
    if((this->ui->le_name->text()=="")||(this->ui->le_nodel->text()=="")||(this->ui->le_number->text()=="")||(this->ui->dsp_price->value()==0.0)){
        QMessageBox::critical(this,"错误","请填写完整信息");
    }else{
        Global *temp=Global::getInstant();

        QString t;
        QDateTime local(QDateTime::currentDateTime());
        QString id="YQ"+local.toString("yyMMddhhmmss")+temp->getUserid().remove(0,2);
        QString data=id+" "+this->ui->le_name->text()+" "+this->ui->le_number->text()+" "+this->ui->le_nodel->text()+
                " "+t.setNum(this->ui->dsp_price->value())+" "+local.toString("yyyy-MM-dd hh:mm")+"\n";
        writeFile("baseinformation/instrument-unused.txt",data);
        QString usinglog=local.toString("yyyy-MM-dd hh:mm")+" 入库 DE1001 "+temp->getUserid()+" "+temp->getUserid()+"\n";
        writeFile(id+".txt",usinglog);
        Hander *hand=Hander::getInstant();
        hand->sendName(this->ui->le_name->text());
        this->ui->le_name->clear();
        this->ui->le_nodel->clear();
        this->ui->le_number->clear();
        this->ui->dsp_price->setValue(0);
        this->ui->le_name->setFocus();
    }
}

void AddInstrument::writeFile(QString path, QString data)
{
    QFile file(path);
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << data;
    file.close();
}

void AddInstrument::on_le_nodel_returnPressed()
{
    this->ui->dsp_price->clear();
    this->ui->dsp_price->setFocus();
}

void AddInstrument::on_dsp_price_editingFinished()
{
    this->ui->pb_ok->click();
}
