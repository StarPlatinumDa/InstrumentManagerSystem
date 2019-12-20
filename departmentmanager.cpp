#include "departmentmanager.h"
#include "ui_departmentmanager.h"
#include<QFile>
#include<QTextStream>
#include<department.h>
DepartmentManager::DepartmentManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DepartmentManager)
{
    ui->setupUi(this);
    //读入负责人文件
    QFile file1("baseinformation/user.txt");
    file1.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream stream1(&file1);
    fuzeren=new QStringList;
    while(!stream1.atEnd())
    {
        fuzeren->append(stream1.readLine());
    }
    //读入部门文件
    QFile file("baseinformation/department.txt");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream stream(&file);
    str=new QStringList;
    while(!stream.atEnd())
    {
        str->append(stream.readLine());

    }
    qDebug()<<str->length();
    this->ui->cb_master->setCurrentIndex(-1);
    this->ui->cb_department->setCurrentIndex(-1);
    model=new QStandardItemModel;
    this->ui->tableView->setModel(model);
    this->ui->cb_department->addItem("全部");
    this->ui->cb_master->addItem("全部");
    for(int i=0;i<str->length();i++)
    {
        this->ui->cb_department->addItem(str->at(i).split(" ").at(1));
        this->ui->cb_master->addItem(searchfuzerenB(str->at(i).split(" ").at(2)));
    }
    show(str);
    connect(&m,&Modify::addcomplete,this,&DepartmentManager::dealnextadd);//下个界面新增的槽函数

}

DepartmentManager::~DepartmentManager()
{
    delete ui;
    delete str;
}

void DepartmentManager::head()
{
    model->setHorizontalHeaderItem(0,new QStandardItem("部门编号"));
    model->setHorizontalHeaderItem(1,new QStandardItem("名称"));
    model->setHorizontalHeaderItem(2,new QStandardItem("部门负责人"));
    model->setHorizontalHeaderItem(3,new QStandardItem("部门描述"));
    this->ui->tableView->setColumnWidth(0,110);
    this->ui->tableView->setColumnWidth(1,160);
    this->ui->tableView->setColumnWidth(2,220);
    this->ui->tableView->setColumnWidth(3,210);
}

void DepartmentManager::show(QStringList *str)
{
    model->clear();
    for(int i=0;i<str->length();i++)
    {
        model->setItem(i,0,new QStandardItem(str->at(i).split(" ").at(0)));
        model->setItem(i,1,new QStandardItem(str->at(i).split(" ").at(1)));
        model->setItem(i,2,new QStandardItem(searchfuzerenB(str->at(i).split(" ").at(2))));
        model->setItem(i,3,new QStandardItem(str->at(i).split(" ").at(3)));
        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model->item(i,3)->setTextAlignment(Qt::AlignCenter);
    }
    head();
}


void DepartmentManager::searchdepart(QString d)
{
    if(d=="全部"){
        ui->cb_master->setCurrentText("全部");
        show(str);
    }
    else{
        QStringList *p=new QStringList;
        for(int i=0;i<str->length();i++){
            if(d==str->at(i).split(" ").at(1)){
                p->append(str->at(i));
                ui->cb_master->setCurrentText(p->at(0).split(" ").at(2));
                show(p);
            }
        }
       delete p;
    }
}

void DepartmentManager::searchfuzeren(QString d)
{
    if(d=="全部"){
        ui->cb_department->setCurrentText("全部");
        show(str);
    }
        QStringList *p=new QStringList;
        for(int i=0;i<str->length();i++){
            if(d==searchfuzerenB(str->at(i).split(" ").at(2))){
                p->append(str->at(i));
                ui->cb_department->setCurrentText(p->at(0).split(" ").at(1));
                show(p);
            }
        }
       delete p;
    }


QString DepartmentManager::searchfuzerenB(QString f)
{
    for(int i=0;i<fuzeren->length();i++){
        if(f==fuzeren->at(i).split(" ").at(0))return fuzeren->at(i).split(" ").at(1);
    }
    return "";
}


void DepartmentManager::on_cb_department_currentTextChanged(const QString &arg1)
{

    QString temp=arg1;
    searchdepart(temp);
}

void DepartmentManager::on_pb_add_clicked()
{
    m.flag=0;
    m.re();
    m.exec();
}

void DepartmentManager::on_tableView_doubleClicked(const QModelIndex)
{
     searchdepart(this->ui->cb_department->currentText());
}

void DepartmentManager::dealnextadd()
{
    str=m.str;
    ui->cb_department->clear();
    ui->cb_master->clear();
    this->ui->cb_master->setCurrentIndex(-1);
    this->ui->cb_department->setCurrentIndex(-1);
    this->ui->cb_department->addItem("全部");
    this->ui->cb_master->addItem("全部");
    for(int i=0;i<str->length();i++)
    {
        this->ui->cb_department->addItem(str->at(i).split(" ").at(1));
        this->ui->cb_master->addItem(searchfuzerenB(str->at(i).split(" ").at(2)));
    }
    show(str);
}

void DepartmentManager::on_tableView_clicked(const QModelIndex &index)
{
    ui->pb_modify->setEnabled(1);
    m.currentrow=index.row();
}

void DepartmentManager::on_pb_modify_clicked()
{
    m.flag=1;
    m.rebyclick(m.currentrow);
    m.exec();
}

void DepartmentManager::on_cb_master_currentTextChanged(const QString &arg1)
{
    QString temp=arg1;
    searchfuzeren(temp);
}
