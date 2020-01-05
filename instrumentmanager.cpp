#include "instrumentmanager.h"
#include "ui_instrumentmanager.h"
#include "addinstrument.h"
#include "instrumentinfomation.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <hander.h>
#include <QTimer>
#include <detail.h>
#include <global.h>
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
    modelforbaofei=new QStandardItemModel;
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
    QStringList cb_shenheren_items;
    for(int i=0;i<used->length();i++){
        if(cb_shenheren_items.contains(getUserName(used->at(i).split(" ").at(7))))
            continue;
        cb_shenheren_items.append(getUserName(used->at(i).split(" ").at(7)));
    }
    ui->cb_shenheren->addItems(cb_shenheren_items);
    this->ui->tableView_1->setModel(modelforcangku);
    this->ui->tableView_2->setModel(modelfordepartment);
    this->ui->tableView_3->setModel(modelforuser);
    this->ui->tableView_4->setModel(modelforused);
    this->ui->tableView_5->setModel(modelforbaofei);
    this->ui->pb_search_1->click();
    Hander *temp=Hander::getInstant();
    connect(temp,SIGNAL(addInstrument(QString)),this,SLOT(addnew(QString)));
    allequip=new QStringList;
    readFile("baseinformation/instrument-using.txt",allequip);
    readFile("baseinformation/instrument-unused.txt",allequip);
    allaplly=new QStringList;
    readFile("message/message.txt",allaplly);
    modelforapply=new QStandardItemModel;
    ui->tableView_6->setModel(modelforapply);
    useableapply=new QStringList;
    for(int i=0;i<allaplly->length();i++)//分理出部门通过的申请
    {
        if(allaplly->at(i).split(" ").at(0)=="A"||allaplly->at(i).split(" ").at(0)=="D"){
            if(allaplly->at(i).split(" ").at(3)=="1")useableapply->append(allaplly->at(i));
        }
        if(allaplly->at(i).split(" ").at(0)=="C")useableapply->append(allaplly->at(i));
    }
    departname=new QStringList;
    for(int i=0;i<useableapply->length();i++){
        if(!departname->contains(getDepartmentName(getUserDepartmentId(useableapply->at(i).split(" ").at(1)))))departname->append(getDepartmentName(getUserDepartmentId(useableapply->at(i).split(" ").at(1))));
    }
    for(int i=0;i<departname->length();i++){
        ui->cb_department->addItem(departname->at(i));
    }
    showapply(useableapply);
}

InstrumentManager::~InstrumentManager()
{
    delete ui;
    delete users;
    delete useing;
    delete unused;
    delete departments;
    delete used;
    delete modelfordepartment;
    delete modelforcangku;
    delete modelforuser;
    delete modelforused;
    delete modelforbaofei;
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

bool InstrumentManager::writeFile(QString path, QStringList *container)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream out(&file);
    for(auto&i:*container){
        out<<i<<"\n";
    }
    file.close();
    return true;
}
//设置column列的表格，liset的一项代表一行，个列用空格隔开
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

QString InstrumentManager::departmentNameToId(QString name)
{
    QString temp;
    for(int i=0;i<departments->length();i++){
        if(name==departments->at(i).split(" ").at(1)){
            temp=departments->at(i).split(" ").at(0);
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

QStringList InstrumentManager::findForUsed(QString name)
{//审核人 名称 条码 型号 价格
    QStringList result;
    if(name==""){
        for (auto&i:*used) {
            result.append(getUserName(i.split(" ").at(7))+" "+
                          i.split(" ").at(1)+" "+i.split(" ").at(0)+" "+i.split(" ").at(3)+" "+i.split(" ").at(4));
        }
    }else{
        for (auto&i:*useing) {
            if(i.split(" ").at(1).contains(name)){
                result.append(getUserName(i.split(" ").at(7))+" "+
                              i.split(" ").at(1)+" "+i.split(" ").at(0)+" "+i.split(" ").at(3)+" "+i.split(" ").at(4));
            }
        }
    }
    return result;
}

int InstrumentManager::getCanZhi(QString start)
{
    QDate in(start.split("-").at(0).toInt(),start.split("-").at(1).toInt(),1);
    QDate toend(QDateTime::currentDateTime().date().year(),QDateTime::currentDateTime().date().month(),1);
    int months=0;
    while (toend!=in) {
        in=in.addMonths(1);
        months++;
        if(months==61)
            break;
    }
    if(months>60)
        return 0;
    return int((1-months/60.0)*100);
}

QStringList InstrumentManager::findForBaofei()
{//入库时间 设备名称 设备编号 设备型号 设备价格
    QStringList result;
    QString start;
    for(auto&i:*unused){
        if(getCanZhi(i.split(" ").at(5))==0){
            result.append(i.split(" ").at(5)+" "+i.split(" ").at(1)+" "+i.split(" ").at(0)+" "+i.split(" ").at(3)+" "+i.split(" ").at(4));
        }
    }
    return result;
}

void InstrumentManager::refreshBaofei()
{
    modelforbaofei->clear();
    setheader(5,"入库时间 设备名称 设备编号 设备型号 设备价格",modelforbaofei);
    this->ui->tableView_5->setColumnWidth(0,150);
    this->ui->tableView_5->setColumnWidth(1,150);
    this->ui->tableView_5->setColumnWidth(2,150);
    this->ui->tableView_5->setColumnWidth(3,150);
    this->ui->tableView_5->setColumnWidth(4,150);
    QStringList temp=findForBaofei();
    settable(5,modelforbaofei,&temp);
}

void InstrumentManager::headapply()
{
    modelforapply->setHorizontalHeaderItem(0,new QStandardItem("申请类型"));
    modelforapply->setHorizontalHeaderItem(1,new QStandardItem("申请部门"));
    modelforapply->setHorizontalHeaderItem(2,new QStandardItem("申请人"));
    modelforapply->setHorizontalHeaderItem(3,new QStandardItem("仪器名称"));
    modelforapply->setHorizontalHeaderItem(4,new QStandardItem("审核情况"));
    ui->tableView_6->setColumnWidth(0,150);
    ui->tableView_6->setColumnWidth(1,150);
    ui->tableView_6->setColumnWidth(2,150);
    ui->tableView_6->setColumnWidth(3,150);
    ui->tableView_6->setColumnWidth(4,132);
}

void InstrumentManager::showapply(QStringList *str)
{
    for(int i=0;i<str->length();i++)
    {
        modelforapply->clear();
        for(int i=0;i<str->length();i++){
            QString temp;
            if(str->at(i).split(" ").at(0)=="A")temp="设备申请";
            else if(str->at(i).split(" ").at(0)=="D")temp="设备归还";
            else temp="跨部门移交";
            modelforapply->setItem(i,0,new QStandardItem(temp));
            modelforapply->setItem(i,1,new QStandardItem(getDepartmentName(getUserDepartmentId(str->at(i).split(" ").at(1)))));
            modelforapply->setItem(i,2,new QStandardItem(getUserName(str->at(i).split(" ").at(1))));
            modelforapply->setItem(i,3,new QStandardItem(findequipnamebyid(str->at(i).split(" ").at(2),allequip)));
//            if(str->at(i).split(" ").at(0)=="A")modelforapply->setItem(i,3,new QStandardItem(findequipnamebyid(str->at(i).split(" ").at(2),unused)));
//            else modelforapply->setItem(i,3,new QStandardItem(findequipnamebyid(str->at(i).split(" ").at(2),useing)));
            if(str->at(i).split(" ").at(0)=="C"){
                modelforapply->setItem(i,4,new QStandardItem(shenhe(str->at(i).split(" ").at(3).toInt())));
            }else {
                modelforapply->setItem(i,4,new QStandardItem(shenhe(str->at(i).split(" ").at(4).toInt())));
            }
            modelforapply->item(i,0)->setTextAlignment(Qt::AlignCenter);
            modelforapply->item(i,1)->setTextAlignment(Qt::AlignCenter);
            modelforapply->item(i,2)->setTextAlignment(Qt::AlignCenter);
            modelforapply->item(i,3)->setTextAlignment(Qt::AlignCenter);
            modelforapply->item(i,4)->setTextAlignment(Qt::AlignCenter);
        }
        headapply();
    }
}

QString InstrumentManager::findequipnamebyid(QString id,QStringList *str)
{
    for(int i=0;i<str->length();i++){
        if(str->at(i).split(" ").at(0)==id)return str->at(i).split(" ").at(1);
    }
    return  "";
}

QString InstrumentManager::shenhe(int i)
{
    if(i==0)return "未通过";
    else if(i==1)return "通过";
    else return "未审核";
}

void InstrumentManager::writefile(QString filename, QStringList *str)
{
    QString a="";
    for(int i=0;i<str->length();i++)
    {
        a=a+str->at(i)+"\n";
    }
    QFile f1(filename);
    f1.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream st(&f1);
    st<<a;
    f1.close();
}
void InstrumentManager::on_pb_search_1_clicked()
{
    modelforcangku->clear();
    setheader(4,"名称 型号/规格 价格 数量",modelforcangku);
    this->ui->tableView_1->setColumnWidth(0,190);
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
        this->ui->tableView_2->setColumnWidth(2,120);
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
    }else if(index==3){
        ui->pb_search_4->click();
    }else if(index==4){
        refreshBaofei();
        ui->pushButton_2->setEnabled(false);
    }
}

void InstrumentManager::on_pb_search_3_clicked()
{
    QStringList temp=findForUser(this->ui->le_user_name->text());
    modelforuser->clear();
    setheader(5,"用户 所属部门 名称 条码 价格",modelforuser);
    this->ui->tableView_3->setColumnWidth(0,100);
    this->ui->tableView_3->setColumnWidth(1,120);
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
}

void InstrumentManager::on_tableView_1_doubleClicked(const QModelIndex &index)
{
    if(index.column()==3){
        QString temp=modelforcangku->item(index.row(),0)->text()+" "+
                modelforcangku->item(index.row(),1)->text()+" "+
                modelforcangku->item(index.row(),2)->text();
        Detail w;
        w.setStandard(temp);
        w.exec();
    }
}

void InstrumentManager::on_tableView_2_doubleClicked(const QModelIndex &index)
{
    if(index.column()==4){
        QString temp=modelfordepartment->item(index.row(),0)->text()+" "+
                modelfordepartment->item(index.row(),1)->text()+" "+
                departmentNameToId(modelfordepartment->item(index.row(),2)->text())+" "+
                modelfordepartment->item(index.row(),3)->text();
        Detail w;
        w.setStandard(temp);
        w.exec();
    }
}

void InstrumentManager::on_pb_search_4_clicked()
{
    modelforused->clear();
    setheader(5,"审核人 名称 条码 型号 价格",modelforused);
    this->ui->tableView_4->setColumnWidth(0,150);
    this->ui->tableView_4->setColumnWidth(1,150);
    this->ui->tableView_4->setColumnWidth(2,150);
    this->ui->tableView_4->setColumnWidth(3,150);
    this->ui->tableView_4->setColumnWidth(4,150);
    QStringList temp=findForUsed(ui->le_instrument_name_2->text());
    if(ui->cb_shenheren->currentText()=="全部"){
        settable(5,modelforused,&temp);
    }else{
        QStringList temp1;
        for(auto&i:temp){
            if(i.contains(ui->cb_shenheren->currentText())){
                temp1.append(i);
            }
        }
        settable(5,modelforused,&temp1);
    }

}

void InstrumentManager::on_le_instrument_name_2_textChanged(const QString &)
{
    ui->pb_search_4->click();
}

void InstrumentManager::on_cb_shenheren_currentIndexChanged(int)
{
    ui->pb_search_4->click();
}


void InstrumentManager::on_tableView_4_doubleClicked(const QModelIndex &index)
{
    if(index.column()==2){
        InstrumentInfomation w;
        w.setInstrumentId(index.data().toString());
        w.exec();
    }
}

void InstrumentManager::on_tableView_5_doubleClicked(const QModelIndex &index)
{
    if(index.column()==2){
        InstrumentInfomation w;
        w.setInstrumentId(index.data().toString());
        w.exec();
    }
}

void InstrumentManager::on_pushButton_2_clicked()
{
    Global *globle=Global::getInstant();
    for(auto&i:*unused){
        if(i.contains(baofeiid)){
            used->append(i+" "+globle->getUserid());
            unused->removeOne(i);
            break;
        }
    }
    writeFile("baseinformation/instrument-unused.txt",unused);
    writeFile("baseinformation/instrument-used.txt",used);
    QMessageBox::information(this,"成功","报废成功");
    ui->pushButton_2->setEnabled(false);
    refreshBaofei();
}

void InstrumentManager::on_tableView_5_clicked(const QModelIndex &index)
{
    baofeiid=modelforbaofei->item(index.row(),2)->text();
    ui->pushButton_2->setEnabled(true);
}
void InstrumentManager::on_cb_department_currentTextChanged(const QString &arg1)
{
    if(arg1=="全部")showapply(useableapply);
    else{
        QStringList *str=new QStringList;
        for(int i=0;i<useableapply->length();i++){
            if(getDepartmentName(getUserDepartmentId(useableapply->at(i).split(" ").at(1)))==arg1){
                str->append(useableapply->at(i));
            }
        }
        showapply(str);
    }
}

void InstrumentManager::on_tableView_6_clicked(const QModelIndex &index)
{
    ui->pb_agree->setEnabled(true);
    ui->pb_reject->setEnabled(true);
    if(modelforapply->item(index.row(),0)->text() =="跨部门移交"){
        for(int i=0;i<useableapply->length();i++){
            if(findequipnamebyid(useableapply->at(i).split(" ").at(2),useing)==modelforapply->item(index.row(),3)->text()&&getUserName(useableapply->at(i).split(" ").at(1))==modelforapply->item(index.row(),2)->text()){
                currrentapplyrow=i;
                break;
            }
        }
    }
    else if(modelforapply->item(index.row(),0)->text() =="设备申请"){
        for(int i=0;i<useableapply->length();i++){
            if(findequipnamebyid(useableapply->at(i).split(" ").at(2),unused)==modelforapply->item(index.row(),3)->text()&&getUserName(useableapply->at(i).split(" ").at(1))==modelforapply->item(index.row(),2)->text())
            {
                    currrentapplyrow=i;
                    break;
            }
        }
    }
    else{
        for(int i=0;i<useableapply->length();i++){
            if(findequipnamebyid(useableapply->at(i).split(" ").at(2),useing)==modelforapply->item(index.row(),3)->text()&&getUserName(useableapply->at(i).split(" ").at(1))==modelforapply->item(index.row(),2)->text()){
                currrentapplyrow=i;
                break;
            }
        }
    }
}

void InstrumentManager::on_pb_agree_clicked()
{
    Global *glo=Global::getInstant();//获取当前登陆者的信息
    QMessageBox *b=new QMessageBox("再次确认",		///--这里是设置消息框标题
                                   "您真的要执行该操作吗？",						///--这里是设置消息框显示的内容
                                   QMessageBox::Question,							///--这里是在消息框显示的图标
                                   QMessageBox::Ok | QMessageBox::Default,		///---这里是显示消息框上的按钮情况
                                   QMessageBox::Cancel | QMessageBox::Escape,	///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
                                   0);
    int a=b->exec();
    if(a==QMessageBox::Ok){
        if(useableapply->at(currrentapplyrow).split(" ").at(0)=="C"){
            for(int i=0;i<allaplly->length();i++){
                if(allaplly->at(i)==useableapply->at(currrentapplyrow)){
                    QString temp="C "+useableapply->at(currrentapplyrow).split(" ").at(1)+" "+useableapply->at(currrentapplyrow).split(" ").at(2)+" 1 "+useableapply->at(currrentapplyrow).split(" ").at(4)+" 1 "+useableapply->at(currrentapplyrow).split(" ").at(6)+" 1 "+glo->getUserid();
                    allaplly->replace(i,temp);
                    useableapply->replace(currrentapplyrow,temp);
                    QString a="";
                    for(int i=0;i<allaplly->length();i++){
                        a=a+allaplly->at(i)+"\n";
                    }
                    QFile f1("message/message.txt");
                    f1.open(QIODevice::WriteOnly|QIODevice::Text);
                    QTextStream st(&f1);
                    st<<a;
                    f1.close();
                    break;
                }
            }
            //写入记录
            QString fname="usinglog/"+useableapply->at(currrentapplyrow).split(" ").at(2)+".txt";
            QFile f1(fname);
            QString a=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
            a=a+" 移交 "+getUserDepartmentId(useableapply->at(currrentapplyrow).split(" ").at(6))+" "+useableapply->at(currrentapplyrow).split(" ").at(6)+" "+glo->getUserid()+"\n";
            f1.open(QIODevice::Append|QIODevice::Text);
            QTextStream st(&f1);
            st<<a;
            f1.close();
        }
        else if(useableapply->at(currrentapplyrow).split(" ").at(0)=="A"){
            for(int i=0;i<allaplly->length();i++){
                if(allaplly->at(i)==useableapply->at(currrentapplyrow)){
                    QString temp="A "+useableapply->at(currrentapplyrow).split(" ").at(1)+" "+useableapply->at(currrentapplyrow).split(" ").at(2)+" 1 1";
                    allaplly->replace(i,temp);
                    useableapply->replace(currrentapplyrow,temp);
                    QString a="";
                    for(int i=0;i<allaplly->length();i++){
                        a=a+allaplly->at(i)+"\n";
                    }
                    QFile f1("message/message.txt");
                    f1.open(QIODevice::WriteOnly|QIODevice::Text);
                    QTextStream st(&f1);
                    st<<a;
                    f1.close();
                    break;
                }
            }
            //写入记录
            QString fname="usinglog/"+useableapply->at(currrentapplyrow).split(" ").at(2)+".txt";
            QFile f1(fname);
            QString a=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
            a=a+" 借出 "+getUserDepartmentId(useableapply->at(currrentapplyrow).split(" ").at(1))+" "+useableapply->at(currrentapplyrow).split(" ").at(1)+" "+glo->getUserid()+"\n";
            f1.open(QIODevice::Append|QIODevice::Text);
            QTextStream st(&f1);
            st<<a;
            f1.close();
            //将设备从unused移动到using
            QString change;
            for(int i=0;i<unused->length();i++){
                if(unused->at(i).contains(useableapply->at(currrentapplyrow).split(" ").at(2)))
                {
                    change=unused->at(i);
                    unused->removeAt(i);
                    break;
                }
            }
            change+=" "+useableapply->at(currrentapplyrow).split(" ").at(1);
            writefile("baseinformation/instrument-unused.txt",unused);//从unused中移出
            //追加在using中
            useing->append(change);
            writefile("baseinformation/instrument-using.txt",useing);

        }
        else{
            for(int i=0;i<allaplly->length();i++){
                if(allaplly->at(i)==useableapply->at(currrentapplyrow)){
                    QString temp="D "+useableapply->at(currrentapplyrow).split(" ").at(1)+" "+useableapply->at(currrentapplyrow).split(" ").at(2)+" 1 1";
                    allaplly->replace(i,temp);
                    useableapply->replace(currrentapplyrow,temp);
                    QString a="";
                    for(int i=0;i<allaplly->length();i++){
                        a=a+allaplly->at(i)+"\n";
                    }
                    QFile f1("message/message.txt");
                    f1.open(QIODevice::WriteOnly|QIODevice::Text);
                    QTextStream st(&f1);
                    st<<a;
                    f1.close();
                    break;
                }
            }
            //写入记录
            QString fname="usinglog/"+useableapply->at(currrentapplyrow).split(" ").at(2)+".txt";
            QFile f1(fname);
            QString a=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
            a=a+" 归还 "+getUserDepartmentId(useableapply->at(currrentapplyrow).split(" ").at(1))+" "+useableapply->at(currrentapplyrow).split(" ").at(1)+" "+glo->getUserid()+"\n";
            f1.open(QIODevice::Append|QIODevice::Text);
            QTextStream st(&f1);
            st<<a;
            f1.close();
            //从using转移到unused
            QString change;
            for(int i=0;i<useing->length();i++){
                if(useing->at(i).contains(useableapply->at(currrentapplyrow).split(" ").at(2)))
                {
                    change=useing->at(i);
                    useing->removeAt(i);
                    break;
                }
            }
            writefile("baseinformation/instrument-using.txt",useing);//从useing中移出
            QString changfi="";
            for(int i=0;i<=5;i++){
                changfi+=change.split(" ").at(i)+" ";
            }
            changfi+=change.split(" ").at(6);
            //追加在unused中
            unused->append(changfi);
            writefile("baseinformation/instrument-unused.txt",unused);

        }
        showapply(useableapply);
        ui->cb_department->setCurrentIndex(0);
    }
    ui->pb_agree->setEnabled(false);
    ui->pb_reject->setEnabled(false);
}

void InstrumentManager::on_pb_reject_clicked()
{
    QMessageBox *b=new QMessageBox("再次确认",		///--这里是设置消息框标题
                                   "您真的要执行该操作吗？",						///--这里是设置消息框显示的内容
                                   QMessageBox::Question,							///--这里是在消息框显示的图标
                                   QMessageBox::Ok | QMessageBox::Default,		///---这里是显示消息框上的按钮情况
                                   QMessageBox::Cancel | QMessageBox::Escape,	///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
                                   0);
    int a=b->exec();
    if(a==QMessageBox::Ok){
        if(useableapply->at(currrentapplyrow).split(" ").at(0)=="C"){
            for(int i=0;i<allaplly->length();i++){
                if(allaplly->at(i)==useableapply->at(currrentapplyrow)){
                    QString temp="C "+useableapply->at(currrentapplyrow).split(" ").at(1)+" "+useableapply->at(currrentapplyrow).split(" ").at(2)+" 0 "+useableapply->at(currrentapplyrow).split(" ").at(4)+" 1 "+useableapply->at(currrentapplyrow).split(" ").at(6)+" 1";
                    allaplly->replace(i,temp);
                    useableapply->replace(currrentapplyrow,temp);
                    QString a="";
                    for(int i=0;i<allaplly->length();i++){
                        a=a+allaplly->at(i)+"\n";
                    }
                    QFile f1("message/message.txt");
                    f1.open(QIODevice::WriteOnly|QIODevice::Text);
                    QTextStream st(&f1);
                    st<<a;
                    f1.close();
                    break;
                }
            }
        }
        else if(useableapply->at(currrentapplyrow).split(" ").at(0)=="A"){
            for(int i=0;i<allaplly->length();i++){
                if(allaplly->at(i)==useableapply->at(currrentapplyrow)){
                    QString temp="A "+useableapply->at(currrentapplyrow).split(" ").at(1)+" "+useableapply->at(currrentapplyrow).split(" ").at(2)+" 1 0";
                    allaplly->replace(i,temp);
                    useableapply->replace(currrentapplyrow,temp);
                    QString a="";
                    for(int i=0;i<allaplly->length();i++){
                        a=a+allaplly->at(i)+"\n";
                    }
                    QFile f1("message/message.txt");
                    f1.open(QIODevice::WriteOnly|QIODevice::Text);
                    QTextStream st(&f1);
                    st<<a;
                    f1.close();
                    break;
                }
            }
            QString fname="usinglog//"+useableapply->at(currrentapplyrow).split(" ").at(2)+".txt";
            QFile f1(fname);
            QString a=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
            a=a+" 借出 "+getUserDepartmentId(useableapply->at(currrentapplyrow).split(" ").at(1))+" "+useableapply->at(currrentapplyrow).split(" ").at(1)+" "+useableapply->at(currrentapplyrow).split(" ").at(1);
        }
        else{
            for(int i=0;i<allaplly->length();i++){
                if(allaplly->at(i)==useableapply->at(currrentapplyrow)){
                    QString temp="D "+useableapply->at(currrentapplyrow).split(" ").at(1)+" "+useableapply->at(currrentapplyrow).split(" ").at(2)+" 1 0";
                    allaplly->replace(i,temp);
                    useableapply->replace(currrentapplyrow,temp);
                    QString a="";
                    for(int i=0;i<allaplly->length();i++){
                        a=a+allaplly->at(i)+"\n";
                    }
                    QFile f1("message/message.txt");
                    f1.open(QIODevice::WriteOnly|QIODevice::Text);
                    QTextStream st(&f1);
                    st<<a;
                    f1.close();
                    break;
                }
            }
        }
        showapply(useableapply);
        ui->cb_department->setCurrentIndex(0);
    }
    ui->pb_agree->setEnabled(false);
    ui->pb_reject->setEnabled(false);
}
