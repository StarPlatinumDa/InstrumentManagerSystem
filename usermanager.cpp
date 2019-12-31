#include "usermanager.h"
#include "ui_usermanager.h"
#include "userinformation.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

UserManager::UserManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserManager)
{
    ui->setupUi(this);
    model=new QStandardItemModel;
    this->ui->tableView->setModel(model);

    QString uesrAddress = "baseinformation/user.txt";
    userList = new QStringList;
    readFile(uesrAddress,userList);

    QString departAddress = "baseinformation/department.txt";
    departmentList=new QStringList;
    readFile(departAddress,departmentList);

    userinformationAdd = new UserInformation;
    userinformationModify = new UserInformation;
    userinformationAdd->setType(1);
    userinformationModify->setType(2);

    connect(userinformationAdd,SIGNAL(refreshSignal()),this,SLOT(refresh()));
    connect(userinformationModify,SIGNAL(refreshSignal()),this,SLOT(refresh()));

    //绘制用户表
    setHead();
    setConnent(userList);

    //添加部门下拉框
    this->ui->cb_department->addItem("全部");
    for (int i=0;i<this->departmentList->size();i++) {
        this->ui->cb_department->addItem(departmentList->at(i).split(" ").at(1));
    }
    this->ui->cb_department->setCurrentIndex(0);

    //添加权限下拉框
    this->ui->cb_power->addItem("全部");
    this->ui->cb_power->addItem("个人");
    this->ui->cb_power->addItem("管理员");
    this->ui->cb_power->setCurrentIndex(0);
}

UserManager::~UserManager()
{
    delete ui;
    delete model;
    delete userinformationAdd;
    delete userinformationModify;
    delete userList;
    delete departmentList;
}

void UserManager::on_pushButton_clicked()
{
    UserInformation w;
    w.exec();
}

void UserManager::on_pb_add_clicked()
{
    userinformationAdd->setAllUser(getUserList());
    userinformationAdd->setAllDepartMent(getDepartmentList());
    userinformationAdd->setDepartmentBox();
    userinformationAdd->exec();
}

void UserManager::setHead()
{
    model->setHorizontalHeaderItem(0,new QStandardItem("用户编号"));
    model->setHorizontalHeaderItem(1,new QStandardItem("用户姓名"));
    model->setHorizontalHeaderItem(2,new QStandardItem("所属部门"));
    model->setHorizontalHeaderItem(3,new QStandardItem("用户权限"));
    this->ui->tableView->setColumnWidth(0,115);
    this->ui->tableView->setColumnWidth(1,100);
    this->ui->tableView->setColumnWidth(2,150);
    this->ui->tableView->setColumnWidth(3,100);
}

bool UserManager::getDepartByDepartId(const QString& departId,QString& departName){
    QStringList tempList;
    for (int i=0;i<departmentList->size();i++) {
        tempList=departmentList->at(i).split(" ");
        if(tempList.at(0)==departId){
            departName=tempList.at(1);
            return true;
        }
    }
    return false;
}

 bool UserManager::getPowerByNum(const QString& num,QString& power){
    if(num=="1"){
        power="国资管理员";
        return true;
    }else if(num=="2"){
        power="管理员";
        return true;
    }else if(num=="3"){
        power="个人";
        return true;
    }else{
        return false;
    }
}

bool UserManager::readFile(const QString& fileAddress,QStringList *list)
{
    QFile file(fileAddress);
    if(!file.open( QIODevice::ReadOnly|QIODevice::Text)){
        QMessageBox w;
        w.setWindowTitle("提示");
        w.setIcon(QMessageBox::Warning);
        w.setText("文件打开失败");
        w.addButton("确认",QMessageBox::AcceptRole);
        w.exec();
        return false;
    }
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        list->append(stream.readLine());
    }
    file.close();
    return true;
}

bool UserManager::writeFile(const QString& fileAddress,QStringList *list)
{
    QFile file(fileAddress);
    if(!file.open( QIODevice::WriteOnly|QIODevice::Text)){
        QMessageBox w;
        w.setWindowTitle("提示");
        w.setIcon(QMessageBox::Warning);
        w.setText("文件打开失败");
        w.addButton("确认",QMessageBox::AcceptRole);
        w.exec();
        return false;
    }
    QTextStream stream(&file);
    stream<<list->join("\n")<<"\n";
    file.close();
    return true;
}

void UserManager::setConnent(QStringList* connent)
{
    QString departName;
    QString power;
    for(int i=0;i<connent->size();i++){
        QStringList tempList = connent->at(i).split(" ");
        getDepartByDepartId(tempList.at(2),departName);
        getPowerByNum(tempList.at(3),power);
        model->setItem(i,0,new QStandardItem(tempList.at(0)));
        model->setItem(i,1,new QStandardItem(tempList.at(1)));
        model->setItem(i,2,new QStandardItem(departName));
        model->setItem(i,3,new QStandardItem(power));
        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model->item(i,3)->setTextAlignment(Qt::AlignCenter);
    }
}

int UserManager::localSiteInUserList(const QString &username)
{
    int i;
    for(i=0;i<this->userList->size();i++){
        if(userList->at(i).split(" ").at(0)==username){
            break;
        }
    }
    return i;
}

void UserManager::refresh(){
    QString department=this->ui->cb_department->currentText();
    QString power=this->ui->cb_power->currentText();
    showSlectUser(department,power);
}


void UserManager::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();
    if(model->data(model->index(row,2)).toString()!="国资部"){
        this->ui->pb_modify->setEnabled(true);
        this->ui->pb_delete->setEnabled(true);
    }else {
        this->ui->pb_modify->setEnabled(false);
        this->ui->pb_delete->setEnabled(false);
    }
}

void UserManager::on_pb_modify_clicked()
{
    //将manager窗口的用户、部门信息传给子窗口
    userinformationModify->setAllUser(getUserList());
    userinformationModify->setAllDepartMent(getDepartmentList());
    userinformationModify->setDepartmentBox();

    QStringList list;
    int row = this->ui->tableView->currentIndex().row();
    for(int i=0;i<4;i++){
        list.append(model->data(model->index(row,i)).toString());
    }
    //设置要显示的内容
    userinformationModify->setConnent(list.at(0),list.at(1),list.at(2),list.at(3));
    userinformationModify->exec();
}

void UserManager::on_pb_delete_clicked()
{
    QMessageBox w;
    w.setText("确定删除？");
    w.setWindowTitle("提示");
    w.addButton("确定",QMessageBox::AcceptRole);
    w.addButton("取消",QMessageBox::RejectRole);
    w.setIcon(QMessageBox::Warning);
    int re=w.exec();
    if(re==QMessageBox::AcceptRole){
        QString userAddress ="baseinformation/user.txt";
        int row = this->ui->tableView->selectionModel()->currentIndex().row();
        int site=localSiteInUserList(model->data(model->index(row,0)).toString());
        userList->removeAt(site);
        writeFile(userAddress,userList);
        refresh();
    }

}

QStringList *UserManager::getDepartmentList() const
{
    return departmentList;
}

bool UserManager::showSlectUser(const QString &department, const QString &power)
{
    ui->pb_modify->setEnabled(false);//禁用修改删除按键
    ui->pb_delete->setEnabled(false);

    QString userDepartment;//存储某一用户的部门
    QString userPower;//存储某一用户的权限
    QStringList *suitableUser = new QStringList;//通过筛选的用户

    if(department=="全部"&&power=="全部"){
        model->clear();
        setHead();
        setConnent(getUserList());
    }else if(department=="全部"&&power!="全部"){
        for (int i=0;i<userList->size();i++) {
            QStringList tempList = userList->at(i).split(" ");
            getPowerByNum(tempList.at(3),userPower);
            if(userPower.contains(power)){
                suitableUser->append(tempList.join(" "));
            }
        }
        model->clear();
        setHead();
        setConnent(suitableUser);
    }else if(department!="全部"&&power=="全部"){
        for (int i=0;i<userList->size();i++) {
            QStringList tempList = userList->at(i).split(" ");
            getDepartByDepartId(tempList.at(2),userDepartment);
            if(userDepartment==department){
                suitableUser->append(tempList.join(" "));
            }
        }
        model->clear();
        setHead();
        setConnent(suitableUser);
    }else{
        for (int i=0;i<userList->size();i++) {
            QStringList tempList = userList->at(i).split(" ");
            getPowerByNum(tempList.at(3),userPower);
            getDepartByDepartId(tempList.at(2),userDepartment);
            if(userDepartment==department&&userPower.contains(power)){
                suitableUser->append(tempList.join(" "));
            }
        }
        model->clear();
        setHead();
        setConnent(suitableUser);
    }
    delete suitableUser;
    return true;
}

QStringList *UserManager::getUserList() const
{
    return userList;
}
void UserManager::resizeEvent(QResizeEvent*){
    this->ui->tableView->setColumnWidth(0,(this->ui->tableView->width()/20)*6);
    this->ui->tableView->setColumnWidth(1,(this->ui->tableView->width()/20)*3);
    this->ui->tableView->setColumnWidth(2,(this->ui->tableView->width()/20)*4);
    this->ui->tableView->setColumnWidth(3,(this->ui->tableView->width()/20)*4);
}

void UserManager::on_cb_department_activated(const QString &arg1)
{
    QString currentPower=this->ui->cb_power->currentText(); //获取另外一框的选项
    showSlectUser(arg1,currentPower);
}

void UserManager::on_cb_power_activated(const QString &arg1)
{
    QString currentDepartment=this->ui->cb_department->currentText(); //获取另外一框的选项
    showSlectUser(currentDepartment,arg1);
}

