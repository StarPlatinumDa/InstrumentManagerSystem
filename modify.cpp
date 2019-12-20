#include "modify.h"
#include "ui_modify.h"
#include<QFile>
#include<QTextStream>
#include<QTextCodec>
#include<QMessageBox>
#include<QDebug>
Modify::Modify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Modify)
{
    ui->setupUi(this);
    QFile file("baseinformation\\department.txt");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream stream(&file);
    str=new QStringList;
    //int i=0;
    while(!stream.atEnd())
    {
        str->append(stream.readLine());

    }

    QFile file1("baseinformation/user.txt");
    file1.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream stream1(&file1);
    fuzeren=new QStringList;
    while(!stream1.atEnd())
    {
        fuzeren->append(stream1.readLine());
    }
    for(int i=0;i<fuzeren->length();i++)
    {
        this->ui->cb_fuzeren->addItem(fuzeren->at(i).split(" ").at(1));
    }
    this->ui->cb_fuzeren->setCurrentIndex(-1);
}

Modify::~Modify()
{
    delete ui;
}

void Modify::re()
{
    ui->le_name->clear();
    ui->le_number->setText("DE");
    ui->cb_fuzeren->setCurrentIndex(-1);
    ui->te_description->setText("");
    this->setWindowTitle("新增");
}

void Modify::rebyclick(int i)
{
    ui->le_number->setText(str->at(i).split(" ").at(0));
    ui->le_name->setText(str->at(i).split(" ").at(1));
    ui->cb_fuzeren->setCurrentText(searchfuzeren(str->at(i).split(" ").at(2)));
    ui->te_description->setText(str->at(i).split(" ").at(3));
    this->setWindowTitle("修改");
}

QString Modify::searchfuzeren(QString f)//编码转换为名字
{
    for(int i=0;i<fuzeren->length();i++){
        if(f==fuzeren->at(i).split(" ").at(0))return fuzeren->at(i).split(" ").at(1);
    }
    return "";
}

QString Modify::searchdepart(QString f)//名字转换为编码
{
    QString pcode;
    for(int i=0;i<fuzeren->length();i++){
        if(f==fuzeren->at(i).split(" ").at(1)){
            pcode=fuzeren->at(i).split(" ").at(0);
            return pcode;
        }
    }
    return " ";
}

void Modify::on_pb_ok_clicked()
{
    QRegExp reg("^DE\\d{4}");//正则表达式判断部门编号是否正确
    if(flag==0){//新增
        QString code=ui->le_number->text();
        if(reg.exactMatch(code)){
            QFile f("baseinformation\\department.txt");
            f.open(QIODevice::Append);
            QString write=ui->le_number->text()+" "+ui->le_name->text()+" "+searchdepart(ui->cb_fuzeren->currentText())+" "+ui->te_description->toPlainText()+"\n";
            QString write1=ui->le_number->text()+" "+ui->le_name->text()+" "+searchdepart(ui->cb_fuzeren->currentText())+" "+ui->te_description->toPlainText();
            str->append(write1);
            f.close();
            this->close();
        }
        else {
            QMessageBox *b=new QMessageBox("错误信息",		///--这里是设置消息框标题
                                           "请输入正确的部门编号！",						///--这里是设置消息框显示的内容
                                           QMessageBox::Critical,							///--这里是在消息框显示的图标
                                           QMessageBox::Ok | QMessageBox::Default,		///---这里是显示消息框上的按钮情况
                                           QMessageBox::Cancel | QMessageBox::Escape,	///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
                                           0);
            b->show();
        }
        emit addcomplete();
    }
    else{//修改
        QString code=ui->le_number->text();
        if(reg.exactMatch(code)){
            QString write1=ui->le_number->text()+" "+ui->le_name->text()+" "+searchdepart(ui->cb_fuzeren->currentText())+" "+ui->te_description->toPlainText();
            str->replace(currentrow,write1);
            QString a="";
            for(int i=0;i<str->length();i++){
                a=a+str->at(i)+"\n";
            }
            QFile f1("baseinformation\\department.txt");
            f1.open(QIODevice::WriteOnly|QIODevice::Text);
            QTextStream out(&f1);
            out<<a;
            qDebug()<<a;
            f1.close();
            this->close();
            }
        else{
            QMessageBox *b=new QMessageBox("错误信息",		///--这里是设置消息框标题
                                           "请输入正确的部门编号！",						///--这里是设置消息框显示的内容
                                           QMessageBox::Critical,							///--这里是在消息框显示的图标
                                           QMessageBox::Ok | QMessageBox::Default,		///---这里是显示消息框上的按钮情况
                                           QMessageBox::Cancel | QMessageBox::Escape,	///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
                                           0);
            b->show();
        }
        emit addcomplete();
    }
}

void Modify::on_pb_cancel_clicked()
{
    this->close();
}
