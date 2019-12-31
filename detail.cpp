#include "detail.h"
#include "ui_detail.h"
#include <QFile>
#include <QTextStream>
#include "instrumentinfomation.h"
Detail::Detail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Detail)
{
    ui->setupUi(this);
    model=new QStandardItemModel;
    instruments=new QStringList;
    users=new QStringList;
    this->ui->tableView->setModel(model);
}

Detail::~Detail()
{
    delete ui;
    delete model;
    delete instruments;
    delete users;

}

void Detail::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(index.column()==0){
        InstrumentInfomation w;
        w.setInstrumentId(index.data().toString());
        w.exec();
    }

}

QString Detail::getUserName(QString id)
{
    QString temp;
    for(int i=0;i<users->length();i++){
        if(id==users->at(i).split(" ").at(0)){
            temp=users->at(i).split(" ").at(1);
            break;
        }
    }
    return temp;
}

void Detail::setTableWidth(int &type)
{
    if(type==6){
        this->ui->tableView->setColumnWidth(0,160);
        this->ui->tableView->setColumnWidth(1,100);
        this->ui->tableView->setColumnWidth(2,100);
        this->ui->tableView->setColumnWidth(3,80);
        this->ui->tableView->setColumnWidth(4,80);
        this->ui->tableView->setColumnWidth(5,140);
    }else{
        this->ui->tableView->setColumnWidth(0,140);
        this->ui->tableView->setColumnWidth(1,100);
        this->ui->tableView->setColumnWidth(2,90);
        this->ui->tableView->setColumnWidth(3,70);
        this->ui->tableView->setColumnWidth(4,60);
        this->ui->tableView->setColumnWidth(5,120);
        this->ui->tableView->setColumnWidth(6,80);
    }
}
//查找标准
void Detail::setStandard(const QString &value)
{
    standard = value;
    showInstruments();
}


bool Detail::readFile(QString path, QStringList *container)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        container->append(line);
    }
    file.close();
    return true;
}
//设置表头
void Detail::setheader(int column, QString head, QStandardItemModel *model)
{
    for(int i=0;i<column;i++){
        model->setHorizontalHeaderItem(i,new QStandardItem(head.split(" ").at(i)));
    }
}
//将数据加入表格
void Detail::settable(int column, QStandardItemModel *model, QStringList *data)
{
    for (int i=0;i<data->length();i++) {
        for(int j=0;j<column;j++){
            model->setItem(i,j,new QStandardItem(data->at(i).split(" ").at(j)));
            model->item(i,j)->setTextAlignment(Qt::AlignCenter);
        }
        this->ui->tableView->setRowHeight(i,40);
    }
}
//查找匹配的数据
QStringList *Detail::instrument(QStringList *list, QString NMP)
{
    QStringList *temp=new QStringList;
    if(NMP.split(" ").length()==3){
        for(int i=0;i<list->length();i++){
            if((list->at(i).split(" ").at(1)==NMP.split(" ").at(0))&&(list->at(i).split(" ").at(3)==NMP.split(" ").at(1))&&
                    (list->at(i).split(" ").at(4)==NMP.split(" ").at(2))){
                QString one=list->at(i);
                QString one1=one.remove(one.length()-7,6)+"\n"+list->at(i).right(5);
                temp->append(one1);

            }
        }
    }else if(NMP.split(" ").length()==4){
        for(int i=0;i<list->length();i++){
            if((list->at(i).split(" ").at(1)==NMP.split(" ").at(0))&&(list->at(i).split(" ").at(3)==NMP.split(" ").at(1))&&
                    (list->at(i).split(" ").at(4)==NMP.split(" ").at(3))&&
                    (getUserDepartmentId(list->at(i).split(" ").at(7))==NMP.split(" ").at(2))){
                QString one=list->at(i);
                QString one1=one.remove(one.length()-16,15)+"\n"+list->at(i).right(14).left(5)+" "+getUserName(list->at(i).right(8));
                temp->append(one1);

            }
        }
    }
    return  temp;
}

QString Detail::getUserDepartmentId(QString id)
{
    QString temp;
    for(int i=0;i<users->length();i++){
        if(id==users->at(i).split(" ").at(0)){
            temp=users->at(i).split(" ").at(2);
            break;
        }
    }
    return temp;
}

void Detail::showInstruments()
{
    int column=0;
    if(standard.split(" ").length()==3){
        readFile("baseinformation/instrument-unused.txt",instruments);
        setheader(6,"条码 名称 编号 型号 价格 入库时间",model);
        column=6;

    }else if(standard.split(" ").length()==4){
         readFile("baseinformation/instrument-using.txt",instruments);
         readFile("baseinformation/user.txt",users);
         setheader(7,"条码 名称 编号 型号 价格 入库时间 使用者",model);
         column=7;
    }
    settable(column,model,instrument(instruments,standard));
    setTableWidth(column);
}
