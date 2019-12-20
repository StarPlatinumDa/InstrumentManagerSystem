#include "instrumentmanager.h"
#include "ui_instrumentmanager.h"
#include "addinstrument.h"
#include "instrumentinfomation.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <hander.h>
#include <QTimer>
#include <QtDebug>
InstrumentManager::InstrumentManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InstrumentManager)
{
    ui->setupUi(this);
    users=new QStringList;
    useing=new QStringList;
    departments=new QStringList;
    unused=new QStringList;
    modelfordepartment=new QStandardItemModel;
    modelforcangku=new QStandardItemModel;
    modelforuser=new QStandardItemModel;
    modelforused=new QStandardItemModel;
    used=new QStringList;
    if((!readFile("baseinformation/department.txt",departments))||(!readFile("baseinformation/instrument-using.txt",useing))
            ||(!readFile("baseinformation/user.txt",users))||(!readFile("baseinformation/instrument-used.txt",used))){
        QMessageBox::critical(this,"错误","数据文件丢失");
    }
    if(!readFile("baseinformation/instrument-unused.txt",unused)){
        QMessageBox::critical(this,"错误","数据文件丢失");
        QTimer::singleShot(0,this,SLOT(close()));
    }
    for(int i=0;i<departments->length();i++){
        this->ui->cb_department_name->addItem(departments->at(i).split(" ").at(1));
    }
    this->ui->tableView_1->setModel(modelforcangku);
    this->ui->tableView_2->setModel(modelfordepartment);
    this->ui->tableView_3->setModel(modelforuser);
    this->ui->tableView_4->setModel(modelforused);
    this->ui->pb_search_1->click();
    Hander *temp=Hander::getInstant();
    connect(temp,SIGNAL(addInstrument(QString)),this,SLOT(addnew(QString)));
}

InstrumentManager::~InstrumentManager()
{
    delete ui;
    delete users;
    delete useing;
    delete unused;
    delete departments;
    delete modelfordepartment;
    delete modelforcangku;
    delete modelforuser;
    delete used;
    delete modelforused;
}

void InstrumentManager::on_pushButton_3_clicked()
{
    AddInstrument w;
    w.exec();
}

bool InstrumentManager::readFile(QString path, QStringList *container)
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
//设置column列的表头，表头用head传入，各个表头之间用空格隔开
void InstrumentManager::setheader(int column, QString head, QStandardItemModel *model)
{
    for(int i=0;i<column;i++){
        model->setHorizontalHeaderItem(i,new QStandardItem(head.split(" ").at(i)));
    }
}
//设置column列的表格，list的一项代表一行，个列用空格隔开
void InstrumentManager::settable(int column, QStandardItemModel *model, QStringList *data)
{
    for (int i=0;i<data->length();i++) {
        for(int j=0;j<column;j++){
            model->setItem(i,j,new QStandardItem(data->at(i).split(" ").at(j)));
            model->item(i,j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}
//通过部门ID得到部门名字
QString InstrumentManager::getDepartmentName(QString id)
{
    QString temp;
    for(int i=0;i<departments->length();i++){
        if(id==departments->at(i).split(" ").at(0)){
            temp=departments->at(i).split(" ").at(1);
            break;
        }
    }
    return temp;
}
//通过用户ID得到用户名字
QString InstrumentManager::getUserName(QString id)
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
//通过用户ID得到部门ID
QString InstrumentManager::getUserDepartmentId(QString id)
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
//计算相同设备的数量，计算时会删除list中与NMP相同的条目
int InstrumentManager::instrumentNumber(QStringList * list,QString NMP)
{
    int sum=0;
    if(NMP.split(" ").length()==3){
        for(int i=0;i<list->length();i++){
            if((list->at(i).split(" ").at(1)==NMP.split(" ").at(0))&&(list->at(i).split(" ").at(3)==NMP.split(" ").at(1))&&
                    (list->at(i).split(" ").at(4)==NMP.split(" ").at(2))){
                list->removeAt(i);
                i--;
                sum++;
            }
        }
    }else if(NMP.split(" ").length()==4){
        for(int i=0;i<list->length();i++){
            if((list->at(i).split(" ").at(1)==NMP.split(" ").at(0))&&(list->at(i).split(" ").at(3)==NMP.split(" ").at(1))&&
                    (list->at(i).split(" ").at(4)==NMP.split(" ").at(3))&&
                    (getUserDepartmentId(list->at(i).split(" ").at(7))==getUserDepartmentId(NMP.split(" ").at(2)))){
                list->removeAt(i);
                i--;
                sum++;
            }
        }
    }
    return sum;
}

//查找所有的仓库仪器
QStringList InstrumentManager::findForUnused()
{
    QStringList result;
    QStringList unusedlist=*unused;
    QString temp;
    QString temp1;
    while (!unusedlist.empty()) {
        temp=unusedlist.at(0);
        temp=temp.split(" ").at(1)+" "+temp.split(" ").at(3)+" "+temp.split(" ").at(4);
        result.append(temp+" "+temp1.setNum(instrumentNumber(&unusedlist,temp)));
    }
    return result;
}

QStringList InstrumentManager::findForDepartment()
{
    QStringList result;
    QStringList usinglist=*useing;
    QString temp;
    QString temp1;
    QString temp2;
    while (!usinglist.empty()) {
        temp=usinglist.at(0);
        temp2=temp.split(" ").at(1)+" "+temp.split(" ").at(3)+" "+temp.split(" ").at(7)+" "+temp.split(" ").at(4);
        temp=temp.split(" ").at(1)+" "+temp.split(" ").at(3)+" "+getDepartmentName(getUserDepartmentId(temp.split(" ").at(7)))+" "+temp.split(" ").at(4);
        result.append(temp+" "+temp1.setNum(instrumentNumber(&usinglist,temp2)));
    }
    return result;
}

QStringList InstrumentManager::findForUser(QString name)
{
    QStringList result;
    if(name==""){
        for (auto&i:*useing) {
            result.append(getUserName(i.split(" ").at(7))+" "+
                          getDepartmentName(getUserDepartmentId(i.split(" ").at(7)))+" "+i.split(" ").at(1)+" "+i.split(" ").at(0)+" "+i.split(" ").at(4));
        }
    }else{
        for (auto&i:*useing) {
            if(getUserName(i.split(" ").at(7)).contains(name)){
                result.append(getUserName(i.split(" ").at(7))+" "+
                              getDepartmentName(getUserDepartmentId(i.split(" ").at(7)))+" "+i.split(" ").at(1)+" "+i.split(" ").at(0)+" "+i.split(" ").at(4));
            }
        }
    }
    return result;
}
void InstrumentManager::on_pb_search_1_clicked()
{
    modelforcangku->clear();
    setheader(4,"名称 型号/规格 价格 数量",modelforcangku);
    this->ui->tableView_1->setColumnWidth(0,180);
    this->ui->tableView_1->setColumnWidth(1,185);
    this->ui->tableView_1->setColumnWidth(2,180);
    this->ui->tableView_1->setColumnWidth(3,180);
    if(this->ui->le_instrument_name->text()==""){
        QStringList temp=findForUnused();
        this->ui->lab_result_1->setNum(temp.length());
        settable(4,modelforcangku,&temp);
    }else{
        QStringList temp=findForUnused();
        QStringList temp1;
        for(auto&i:temp){
            if(i.split(" ").at(0).contains(this->ui->le_instrument_name->text())){
                temp1.append(i);
            }
        }
        this->ui->lab_result_1->setNum(temp1.length());
        settable(4,modelforcangku,&temp1);
    }
}
void InstrumentManager::on_pb_add_clicked()
{
    AddInstrument w;
    w.exec();
}

void InstrumentManager::addnew(QString name)
{
    unused->clear();
    if(!readFile("baseinformation/instrument-unused.txt",unused)){
        QMessageBox::critical(this,"错误","数据文件丢失");
        return;
    }
    this->ui->le_instrument_name->setText(name);
    this->ui->pb_search_1->click();
}

void InstrumentManager::on_cb_department_name_currentTextChanged(const QString &arg1)
{
    if(this->ui->tabWidget->currentIndex()==1){
        modelfordepartment->clear();
        setheader(5,"名称 型号/规格 所属部门 价格 数量",modelfordepartment);
        this->ui->tableView_2->setColumnWidth(0,160);
        this->ui->tableView_2->setColumnWidth(1,185);
        this->ui->tableView_2->setColumnWidth(2,100);
        this->ui->tableView_2->setColumnWidth(3,140);
        this->ui->tableView_2->setColumnWidth(4,120);
        if(arg1=="全部"){
            QStringList temp=findForDepartment();
            this->ui->lab_result_2->setNum(temp.length());
            settable(5,modelfordepartment,&temp);
        }else{
            QStringList temp=findForDepartment();
            QStringList temp1;
            for(auto&i:temp){
                if(i.split(" ").at(2)==arg1){
                    temp1.append(i);
                }
            }
            this->ui->lab_result_2->setNum(temp1.length());
            settable(5,modelfordepartment,&temp1);
        }
    }
}

void InstrumentManager::on_tabWidget_currentChanged(int index)
{
    if(index==1){
        int temp=this->ui->cb_department_name->currentIndex();
        this->ui->cb_department_name->setCurrentIndex(1);
        this->ui->cb_department_name->setCurrentIndex(temp);
    }else if(index==2){
        this->ui->pb_search_3->click();
    }
}

void InstrumentManager::on_pb_search_3_clicked()
{
    QStringList temp=findForUser(this->ui->le_user_name->text());
    modelforuser->clear();
    setheader(5,"用户 所属部门 名称 条码 价格",modelforuser);
    this->ui->tableView_3->setColumnWidth(0,100);
    this->ui->tableView_3->setColumnWidth(1,100);
    this->ui->tableView_3->setColumnWidth(2,185);
    this->ui->tableView_3->setColumnWidth(3,200);
    this->ui->tableView_3->setColumnWidth(4,120);
    this->ui->lab_result_3->setNum(temp.length());
    settable(5,modelforuser,&temp);

}

void InstrumentManager::on_le_instrument_name_textChanged(const QString)
{
    this->ui->pb_search_1->click();
}

void InstrumentManager::on_le_user_name_textChanged(const QString)
{
    this->ui->pb_search_3->click();
}

void InstrumentManager::on_tableView_3_doubleClicked(const QModelIndex &index)
{
    if(index.column()==3){
        InstrumentInfomation w;
        w.setInstrumentId(index.data().toString());
        w.exec();
    }
    this->ui->pb_search_3->click();
}
