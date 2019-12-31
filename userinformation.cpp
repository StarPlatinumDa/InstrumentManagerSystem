#include "userinformation.h"
#include "ui_userinformation.h"
#include "QFile"
#include "QMessageBox"
#include "QCryptographicHash"
#include <QTextStream>

UserInformation::UserInformation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInformation)
{
    ui->setupUi(this);
    ui->cb_power->addItem("个人");
    ui->cb_power->addItem("管理员");
    iniPassword="123456";
}

UserInformation::~UserInformation()
{
    delete ui;
}

void UserInformation::on_pb_cancel_clicked()
{
    if(this->ui->le_username->text().isEmpty()&&this->ui->le_name->text().isEmpty()){
        this->close();
    }else{
        QMessageBox w;
        w.setIcon(QMessageBox::Warning);
        w.setText("确认取消？");
        w.addButton("确定",QMessageBox::AcceptRole);
        w.addButton("取消",QMessageBox::RejectRole);
        int re=w.exec();
        if(re==QMessageBox::AcceptRole){
            this->ui->le_username->setText("");
            this->ui->le_name->setText("");
            this->close();
        }
    }

}

void UserInformation::on_pb_ok_clicked()
{
    QString userName=this->ui->le_username->text();
    if(this->type==1){
        if(!(userName.at(0)=='U'&&userName.at(1)=='H'&&userName.length()==8&&!(this->ui->le_name->text().isEmpty()))){
            QMessageBox mess;
            mess.setIcon(QMessageBox::Warning);
            mess.about(this,"提示","账号输入非法或用户名为空");
            mess.show();
        }else if(isRepeatedUserName(userName)){
           QMessageBox mess2;
           mess2.setIcon(QMessageBox::Warning);
           mess2.about(this,"提示","账号重复");
           mess2.show();
        }else{//执行保存并关闭窗口
            QString password;
            encode(iniPassword,password);
            QString departId;
            getDepartIdByDepartName(ui->cb_department->currentText(),departId);
            QString power;
            getPowerNum(ui->cb_power->currentText(),power);
            QString userinfo=ui->le_username->text()+" "+ui->le_name->text()+" "+departId+" "+power+" "+password;

            //同时写入内存和文件
            allUser->append(userinfo);
            QString address="baseinformation/user.txt";
            writeFile(address,allUser);

            emit refreshSignal();
            this->close();
        }
    }else if(this->type==2){
        QMessageBox w;
        w.setWindowTitle("提示");
        w.setIcon(QMessageBox::Warning);
        w.setText("是否确认修改\n用户："+this->ui->le_name->text());
        w.addButton("是",QMessageBox::AcceptRole);
        w.addButton("否",QMessageBox::RejectRole);
        int re = w.exec();
        if(re==QMessageBox::AcceptRole){
            QString userName=this->ui->le_username->text();
            QStringList tempList;
            for (int i=0;i<allUser->size();i++) {
                tempList=allUser->at(i).split(" ");
                if(tempList.at(0)==userName){ //找到对应用户
                    QString departmentId,powerNum;
                    QString address="baseinformation/user.txt";
                    tempList.replace(1,this->ui->le_name->text());
                    getDepartIdByDepartName(this->ui->cb_department->currentText(),departmentId);
                    getPowerNum(this->ui->cb_power->currentText(),powerNum);
                    tempList.replace(2,departmentId);
                    tempList.replace(3,powerNum);
                    allUser->replace(i,tempList.join(" "));
                    writeFile(address,allUser);

                    emit refreshSignal();
                    this->close();
                }
            }
        }
   }
}

QStringList *UserInformation::getAllDepartMent() const
{
    return allDepartMent;
}

void UserInformation::setAllDepartMent(QStringList *value)
{
    allDepartMent = value;
}

void UserInformation::setDepartmentBox()
{
    for(int i=0;i<allDepartMent->size();i++){
        QStringList tempList=allDepartMent->at(i).split(" ");
        if(tempList.at(1)!="国资部"){
            ui->cb_department->addItem(tempList.at(1));
        }
    }
}

void UserInformation::setConnent(const QString &userName, const QString &name, const QString &department, const QString &power)
{
    this->ui->le_username->setEnabled(true);
    this->ui->le_username->setText(userName);
    this->ui->le_name->setText(name);
    this->ui->cb_department->setCurrentText(department);
    this->ui->cb_power->setCurrentText(power);
    this->ui->le_username->setEnabled(false);
}

int UserInformation::localSiteInUserList(const QString &username)
{
    int i;
    for(i=0;i<this->allUser->size();i++){
        if(allUser->at(i).split(" ").at(0)==username){
            break;
        }
    }
    return i;
}

bool UserInformation::readFile(const QString &fileAddress, QStringList *list)
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

bool UserInformation::writeFile(const QString &fileAddress, QStringList *list)
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

int UserInformation::getType() const
{
    return type;
}

void UserInformation::setType(int value)
{
    type = value;
    if(type==2){
        this->setWindowTitle("修改用户信息");
        this->ui->pb_ok->setText("修改");
        this->ui->le_username->setEnabled(false);
    }
    if(type==1){
        this->ui->pb_reset->hide();
    }
}

void UserInformation::setAllUser(QStringList *value)
{
    allUser=value;
}

bool UserInformation::isRepeatedUserName(QString& userName){
    for(int i=0;i<allUser->size();i++){
        if(allUser->at(i).split(" ").at(0)==userName){
            return true;//是重复用户名
        }
    }
    return false;
}

bool UserInformation::encode(const QString &s,QString& p){
    QByteArray ps;
    ps.append(s);
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(ps);
    QByteArray result=hash.result();
    QString md5 = result.toHex();
    p=md5;
    return true;
}

bool UserInformation::getDepartIdByDepartName(const QString& name,QString& num){
    QStringList tempList;
    for(int i=0;i<allUser->size();i++){
        tempList=allUser->at(i).split(" ");
        if(tempList.at(1)==name){
            num=tempList.at(0);
            return true;
        }
    }
    return false;
}

bool UserInformation::getPowerNum(const QString &power,QString& num){
    if(power=="管理员"){
        num="2";
        return true;
    }else if(power=="个人"){
        num="3";
        return true;
    }else if(power=="国资管理员"){
        num="1";
        return true;
    }else{
        return false;
    }
}

void UserInformation::on_pb_reset_clicked()
{
    QMessageBox w;
    w.setWindowTitle("警告");
    w.setIcon(QMessageBox::Warning);
    w.setText("是否重置密码\n用户："+this->ui->le_name->text());
    w.addButton("是",QMessageBox::AcceptRole);
    w.addButton("否",QMessageBox::RejectRole);
    int re = w.exec();
    if(re==QMessageBox::AcceptRole){
        QString address="baseinformation/user.txt";
        QString password;
        int site=localSiteInUserList(ui->le_username->text());
        QStringList temp = allUser->at(site).split(" ");
        encode(iniPassword,password);
        temp.replace(4,password);
        allUser->replace(site,temp.join(" "));
        writeFile(address,allUser);
        QMessageBox p;
        p.setWindowTitle("提示");
        p.setIcon(QMessageBox::Information);
        p.setText("密码重置成功");
        p.exec();
        this->close();
    }
}
