#include "sub1.h"
#include "ui_sub1.h"
#include"dialog.h"
#include<QFile>
#include<QTextStream>
#include"global.h"
#include<QMessageBox>
Sub1::Sub1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sub1)
{
    ui->setupUi(this);
    //全部默认设在第一个界面
    ui->stackMain->setCurrentIndex(0);

    Global *temp=Global::getInstant();
    allapply=new QStringList;//读取申请个数
    readfile("message/message.txt",allapply);
    thisapply=new QStringList;
    for(int i=0;i<allapply->length();i++){
        //获取本部门所有信息
        if(temp->getInstrumentName()==finddepartbyid(allapply->at(i).split(" ").at(1))){
            thisapply->append(allapply->at(i));
        }
    }
    //获取本部门所有设备申请信息
    useapply=new QStringList;
    for(int i=0;i<thisapply->length();i++){
        //获取本部门所有信息
        if(thisapply->at(i).split(" ").at(0)=="A"){
            useapply->append(thisapply->at(i));
        }
    }
    //获取本部门所有设备归还信息
    backapply=new QStringList;
    for(int i=0;i<thisapply->length();i++){
        //获取本部门所有信息
        if(thisapply->at(i).split(" ").at(0)=="D"){
           backapply->append(thisapply->at(i));
        }
    }

    //设备申请
    modeluse=new QStandardItemModel;
    ui->tableView_4->setModel(modeluse);
    showuse(useapply);
    ui->comboBox_3->addItem("全部");
    for(int i=0;i<useapply->length();i++)
    {
        this->ui->comboBox_3->addItem(findnamebyid(useapply->at(i).split(" ").at(1)));
    }
    ui->comboBox_3->setCurrentIndex(-1);

    //设备归还
    modelback=new QStandardItemModel;
    ui->tableView_5->setModel(modelback);
    showback(backapply);
    ui->comboBox_4->setCurrentIndex(-1);
    ui->comboBox_4->addItem("全部");
    for(int i=0;i<backapply->length();i++)
    {
        this->ui->comboBox_4->addItem(findnamebyid(backapply->at(i).split(" ").at(1)));
    }


    //second
    modelUser=new QStandardItemModel;
        modelInstrument=new QStandardItemModel;
        this->ui->tableView_2->setModel(modelUser);
        this->ui->tableView_3->setModel(modelInstrument);
        usingList = new QStringList;
        departMember = new QStringList;
        userList = new QStringList;
        instrumentList = new QStringList;
        messageList = new QStringList;
        allUser = new QStringList;
        dialogDepart = new Dialog;
        dialogCrossDepart = new Dialog;

        QString userAddress = "baseinformation/user.txt";
        QString usingAddress = "baseinformation/instrument-using.txt";
        QString messageAddress = "message/message.txt";

        connect(dialogDepart,SIGNAL(refreshSignal()),this,SLOT(refresh()));
        connect(dialogCrossDepart,SIGNAL(refreshSignal()),this,SLOT(refresh()));

        readFile(userAddress,allUser);
        readFile(usingAddress,usingList);
        readFile(messageAddress,messageList);

        QString departNum;

        QString userId = Global::getInstant()->getUserid();

        getLoginDepart(userId,departNum);
        for (int i=0;i<allUser->size();i++) {
            QStringList tempList=allUser->at(i).split(" ");
            if(departNum!=tempList.at(2)){
                continue;
            }
            QString userOne = tempList.at(0)+" "+tempList.at(1);//userList其中一个用户
            int count=0;//统计某种仪器的数量
            QString temp;
            for(int i=0;i<usingList->size();i++){
                if(usingList->at(i).split(" ").at(7)==tempList.at(0)){
                    count++;
                }
            }
            QString kount=QString::number(count);
            userOne = userOne + " "+kount;
            userList->append(userOne);
        }

        setConnent(userList,1);
        this->ui->label_5->setText(QString::number(userList->size()));

        for (int i=0;i<usingList->size();i++) {
            QString oneInfo=usingList->at(i);
            if(isBelongToDepart(oneInfo)){
                QStringList tempList = usingList->at(i).split(" ");
                QStringList oneInstrument;
                QString name;
                QString id=tempList.at(7);
                getNameById(name,id);
                oneInstrument.append(tempList.at(0));
                oneInstrument.append(tempList.at(1));
                oneInstrument.append(tempList.at(3));
                oneInstrument.append(tempList.at(4));
                oneInstrument.append(name);
                instrumentList->append(oneInstrument.join(" "));
            }

        }

        setConnent(instrumentList,2);
        this->ui->label_7->setText(QString::number(instrumentList->size()));
        setHead(1);
        setHead(2);
}

Sub1::~Sub1()
{
    delete ui;
}

void Sub1::readfile(QString filename, QStringList *str)
{
    QFile f1(filename);
    f1.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream stream1(&f1);
    while(!stream1.atEnd())
    {
        str->append(stream1.readLine());
    }
    f1.close();
}

QString Sub1::finddepartbyid(QString id){
    QStringList *str=new QStringList;
    readfile("baseinformation/user.txt",str);
    for(int i=0;i<str->length();i++){
        if(id==str->at(i).split(" ").at(0)){
            return str->at(i).split(" ").at(2);
        }
    }
    return "";
}

void Sub1::headuse()
{
    modeluse->setHorizontalHeaderItem(0,new QStandardItem("申请人"));
    modeluse->setHorizontalHeaderItem(1,new QStandardItem("设备编码"));
    modeluse->setHorizontalHeaderItem(2,new QStandardItem("设备名称"));
    modeluse->setHorizontalHeaderItem(3,new QStandardItem("部门审核情况"));
    this->ui->tableView_4->setColumnWidth(0,120);
    this->ui->tableView_4->setColumnWidth(1,180);
    this->ui->tableView_4->setColumnWidth(2,240);
    this->ui->tableView_4->setColumnWidth(3,150);
}

void Sub1::headback()
{
    modelback->setHorizontalHeaderItem(0,new QStandardItem("申请人"));
    modelback->setHorizontalHeaderItem(1,new QStandardItem("设备编码"));
    modelback->setHorizontalHeaderItem(2,new QStandardItem("设备名称"));
    modelback->setHorizontalHeaderItem(3,new QStandardItem("部门审核情况"));
    this->ui->tableView_5->setColumnWidth(0,120);
    this->ui->tableView_5->setColumnWidth(1,180);
    this->ui->tableView_5->setColumnWidth(2,240);
    this->ui->tableView_5->setColumnWidth(3,150);
}

void Sub1::showuse(QStringList *str)
{
    modeluse->clear();
    for(int i=0;i<str->length();i++){
        modeluse->setItem(i,0,new QStandardItem(findnamebyid(str->at(i).split(" ").at(1))));
        modeluse->setItem(i,1,new QStandardItem(str->at(i).split(" ").at(2)));
        modeluse->setItem(i,2,new QStandardItem(findusenamebyequipid(str->at(i).split(" ").at(2))));
        QString a;
        if(str->at(i).split(" ").at(3)=="0")a="未通过";
        else if(str->at(i).split(" ").at(3)=="-1")a="未审核";
        else a="通过";
        modeluse->setItem(i,3,new QStandardItem(a));
        modeluse->item(i,0)->setTextAlignment(Qt::AlignCenter);
        modeluse->item(i,1)->setTextAlignment(Qt::AlignCenter);
        modeluse->item(i,2)->setTextAlignment(Qt::AlignCenter);
        modeluse->item(i,3)->setTextAlignment(Qt::AlignCenter);
    }
    headuse();
}

void Sub1::showback(QStringList *str)
{
    modelback->clear();
    for(int i=0;i<str->length();i++){
        modelback->setItem(i,0,new QStandardItem(findnamebyid(str->at(i).split(" ").at(1))));
        modelback->setItem(i,1,new QStandardItem(str->at(i).split(" ").at(2)));
        modelback->setItem(i,2,new QStandardItem(findbacknamebyequipid(str->at(i).split(" ").at(2))));
        QString a;
        if(str->at(i).split(" ").at(3)=="0")a="未通过";
        else if(str->at(i).split(" ").at(3)=="-1")a="未审核";
        else a="通过";
        modelback->setItem(i,3,new QStandardItem(a));
        modelback->item(i,0)->setTextAlignment(Qt::AlignCenter);
        modelback->item(i,1)->setTextAlignment(Qt::AlignCenter);
        modelback->item(i,2)->setTextAlignment(Qt::AlignCenter);
        modelback->item(i,3)->setTextAlignment(Qt::AlignCenter);
    }
    headback();
}

QString Sub1::findnamebyid(QString id)
{
    QStringList *str=new QStringList;
    readfile("baseinformation/user.txt",str);
    for(int i=0;i<str->length();i++){
        if(id==str->at(i).split(" ").at(0)){
            return str->at(i).split(" ").at(1);
        }
    }
    return "";
}

QString Sub1::findidbyname(QString name)
{
    QStringList *str=new QStringList;
    readfile("baseinformation/user.txt",str);
    for(int i=0;i<str->length();i++){
        if(name==str->at(i).split(" ").at(1)){
            return str->at(i).split(" ").at(0);
        }
    }
    return "";
}

QString Sub1::findusenamebyequipid(QString id)
{
    QStringList *str=new QStringList;
    readfile("baseinformation/instrument-unused.txt",str);
    for(int i=0;i<str->length();i++){
        if(id==str->at(i).split(" ").at(0)){
            return str->at(i).split(" ").at(1);
        }
    }
    return "";
}

QString Sub1::findbacknamebyequipid(QString id)
{
    QStringList *str=new QStringList;
    readfile("baseinformation/instrument-using.txt",str);
    for(int i=0;i<str->length();i++){
        if(id==str->at(i).split(" ").at(0)){
            return str->at(i).split(" ").at(1);
        }
    }
    return "";
}

void Sub1::searchuse(QString str)
{
    if(str=="全部"){
        ui->comboBox_3->setCurrentText("全部");
        showuse(useapply);
    }
    else{
        QStringList *p=new QStringList;
        for(int i=0;i<useapply->length();i++){
            if(findidbyname(str)==useapply->at(i).split(" ").at(1)){
                p->append(useapply->at(i));
                showuse(p);
            }
        }
       delete p;
    }
}

void Sub1::searchback(QString str)
{
    if(str=="全部"){
        ui->comboBox_3->setCurrentText("全部");
        showback(backapply);
    }
    else{
        QStringList *p=new QStringList;
        for(int i=0;i<backapply->length();i++){
            if(findidbyname(str)==backapply->at(i).split(" ").at(1)){
                p->append(backapply->at(i));
                showback(p);
            }
        }
       delete p;
    }
}

bool Sub1::readFile(const QString &fileAddress, QStringList *list)
{
    QFile file(fileAddress);
        if(!file.open( QIODevice::ReadOnly|QIODevice::Text)){
            return false;
        }
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            list->append(stream.readLine());
        }
        file.close();
        return true;
}

bool Sub1::writeFile(const QString &fileAddress, QStringList *list)
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

void Sub1::setHead(int category)
{
    if(category==1){
            modelUser->setHorizontalHeaderItem(0,new QStandardItem("用户编号"));
            modelUser->setHorizontalHeaderItem(1,new QStandardItem("用户姓名"));
            modelUser->setHorizontalHeaderItem(2,new QStandardItem("拥有设备数量"));
            this->ui->tableView_2->setColumnWidth(0,300);
            this->ui->tableView_2->setColumnWidth(1,200);
            this->ui->tableView_2->setColumnWidth(2,200);
        }else if(category==2){
            modelInstrument->setHorizontalHeaderItem(0,new QStandardItem("设备编号"));
            modelInstrument->setHorizontalHeaderItem(1,new QStandardItem("设备名称"));
            modelInstrument->setHorizontalHeaderItem(2,new QStandardItem("型号"));
            modelInstrument->setHorizontalHeaderItem(3,new QStandardItem("价格"));
            modelInstrument->setHorizontalHeaderItem(4,new QStandardItem("当前使用人"));
            this->ui->tableView_3->setColumnWidth(0,230);
            this->ui->tableView_3->setColumnWidth(1,180);
            this->ui->tableView_3->setColumnWidth(2,70);
            this->ui->tableView_3->setColumnWidth(3,100);
            this->ui->tableView_3->setColumnWidth(4,130);
    }
}

void Sub1::setConnent(QStringList *connent, int category)
{
    if(category==1){
            modelUser->clear();
            for(int i=0;i<connent->size();i++){
                QStringList tempList = connent->at(i).split(" ");
                modelUser->setItem(i,0,new QStandardItem(tempList.at(0)));
                modelUser->setItem(i,1,new QStandardItem(tempList.at(1)));
                modelUser->setItem(i,2,new QStandardItem(tempList.at(2)));

                modelUser->item(i,0)->setTextAlignment(Qt::AlignCenter);
                modelUser->item(i,1)->setTextAlignment(Qt::AlignCenter);
                modelUser->item(i,2)->setTextAlignment(Qt::AlignCenter);
            }
        }else if(category==2){
            modelInstrument->clear();
            for(int i=0;i<connent->size();i++){
                QStringList tempList = connent->at(i).split(" ");
                modelInstrument->setItem(i,0,new QStandardItem(tempList.at(0)));
                modelInstrument->setItem(i,1,new QStandardItem(tempList.at(1)));
                modelInstrument->setItem(i,2,new QStandardItem(tempList.at(2)));
                modelInstrument->setItem(i,3,new QStandardItem(tempList.at(3)));
                modelInstrument->setItem(i,4,new QStandardItem(tempList.at(4)));
                modelInstrument->item(i,0)->setTextAlignment(Qt::AlignCenter);
                modelInstrument->item(i,1)->setTextAlignment(Qt::AlignCenter);
                modelInstrument->item(i,2)->setTextAlignment(Qt::AlignCenter);
                modelInstrument->item(i,3)->setTextAlignment(Qt::AlignCenter);
                modelInstrument->item(i,4)->setTextAlignment(Qt::AlignCenter);
            }
    }
}

void Sub1::getLoginDepart(QString &userName, QString &departNum)
{
    for (int i=0;i<allUser->size();i++) {
            if(allUser->at(i).split(" ").at(0)==userName){
                departNum=allUser->at(i).split(" ").at(2);
                break;
            }
    }
}

bool Sub1::isBelongToDepart(QString &oneInfo)
{
    QStringList tempList = oneInfo.split(" ");
        for(int i=0;i<userList->size();i++){
            QStringList tempList2 = userList->at(i).split(" ");
            if(tempList.at(7)==tempList2.at(0)){
                return true;
            }
        }
        return false;
}

void Sub1::getNameById(QString &name, QString &id)
{
    for(int i=0;i<allUser->size();i++){
            QStringList tempList = allUser->at(i).split(" ");
            if(id==tempList.at(0)){
                name=tempList.at(1);
                break;
            }
    }
}

int Sub1::checkState(QString instrumentId)
{
    for(int i=0;i<messageList->size();i++){
            QStringList tempList = messageList->at(i).split(" ");
            if(tempList.at(2)==instrumentId){
                if(tempList.at(0)=="B"){
                    return 2;
                }else if(tempList.at(0)=="C"){
                    return 3;
                }
            }
        }
        return 1;
}

void Sub1::on_pb_user_clicked()
{
    ui->stackMain->setCurrentIndex(0);
}

void Sub1::on_pb_esearch_clicked()
{
    ui->stackMain->setCurrentIndex(1);
}

void Sub1::on_pb_emanage_clicked()
{
    ui->stackMain->setCurrentIndex(2);
}

void Sub1::on_pushButton_2_clicked()
{
    dialogCrossDepart->empty();
        dialogCrossDepart->setSomeEnable(true);
        int row = this->ui->tableView_3->currentIndex().row();
        dialogCrossDepart->setAtributes(messageList,allUser,modelInstrument->data(modelInstrument->index(row,0)).toString());
        dialogCrossDepart->exec();
}

void Sub1::on_pushButton_clicked()
{
    dialogDepart->empty();
        dialogDepart->setSomeEnable(false);
        int row = this->ui->tableView_3->currentIndex().row();
        dialogDepart->setAtributes(messageList,allUser,modelInstrument->data(modelInstrument->index(row,0)).toString());
        dialogDepart->exec();
}

void Sub1::on_comboBox_3_currentTextChanged(const QString &arg1)
{
    searchuse(arg1);
}

void Sub1::on_tableView_4_clicked(const QModelIndex &index)
{
    ui->pushButton_13->setEnabled(true);
    ui->pushButton_14->setEnabled(true);
    QModelIndex in=modeluse->index(index.row(),0);
    int i;
    for(i=0;i<useapply->length();i++)
    {
        if(useapply->at(i).split(" ").at(1)==findidbyname(in.data().toString()))break;
    }
    currentrowuse=i;
}

void Sub1::on_pushButton_13_clicked()
{
    QMessageBox *b=new QMessageBox("再次确认",		///--这里是设置消息框标题
                                   "您真的要执行该操作吗？",						///--这里是设置消息框显示的内容
                                   QMessageBox::Question,							///--这里是在消息框显示的图标
                                   QMessageBox::Ok | QMessageBox::Default,		///---这里是显示消息框上的按钮情况
                                   QMessageBox::Cancel | QMessageBox::Escape,	///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
                                   0);
    int a=b->exec();
    if(a==QMessageBox::Ok){
        for(int i=0;i<allapply->length();i++){
            if(useapply->at(currentrowuse)==allapply->at(i))
            {
                QString t="";
                for(int i=0;i<3;i++)
                {
                    t=t+useapply->at(currentrowuse).split(" ").at(i)+" ";
                }
                t=t+"1"+" -1";
                useapply->replace(currentrowuse,t);
                allapply->replace(i,t);
                QString a="";
                for(int i=0;i<allapply->length();i++){
                    a=a+allapply->at(i)+"\n";
                }
                QFile f1("message/message.txt");
                f1.open(QIODevice::WriteOnly|QIODevice::Text);
                QTextStream st(&f1);
                st<<a;
                f1.close();
            }
        }
        showuse(useapply);
        ui->comboBox_3->setCurrentIndex(0);
    }
    ui->pushButton_13->setEnabled(false);
    ui->pushButton_14->setEnabled(false);
}

void Sub1::on_pushButton_14_clicked()
{
    QMessageBox *b=new QMessageBox("再次确认",		///--这里是设置消息框标题
                                   "您真的要执行该操作吗？",						///--这里是设置消息框显示的内容
                                   QMessageBox::Question,							///--这里是在消息框显示的图标
                                   QMessageBox::Ok | QMessageBox::Default,		///---这里是显示消息框上的按钮情况
                                   QMessageBox::Cancel | QMessageBox::Escape,	///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
                                   0);
    int a=b->exec();
    if(a==QMessageBox::Ok){
        for(int i=0;i<allapply->length();i++){
            if(useapply->at(currentrowuse)==allapply->at(i))
            {
                QString t="";
                for(int i=0;i<3;i++)
                {
                    t=t+useapply->at(currentrowuse).split(" ").at(i)+" ";
                }
                t=t+"0"+" -1";
                useapply->replace(currentrowuse,t);
                allapply->replace(i,t);
                QString a="";
                for(int i=0;i<allapply->length();i++){
                    a=a+allapply->at(i)+"\n";
                }
                QFile f1("message/message.txt");
                f1.open(QIODevice::WriteOnly|QIODevice::Text);
                QTextStream st(&f1);
                st<<a;
                f1.close();
            }
        }
        showuse(useapply);
        ui->comboBox_3->setCurrentIndex(0);
    }
    ui->pushButton_13->setEnabled(false);
    ui->pushButton_14->setEnabled(false);
}

void Sub1::on_comboBox_4_currentTextChanged(const QString &arg1)
{
    searchback(arg1);
}

void Sub1::on_tableView_5_clicked(const QModelIndex &index)
{
    ui->pushButton_9->setEnabled(true);
    QModelIndex in=modelback->index(index.row(),0);
    int i;
    for(i=0;i<backapply->length();i++)
    {
        if(backapply->at(i).split(" ").at(1)==findidbyname(in.data().toString()))break;
    }
    currentrowback=i;
}

void Sub1::on_pushButton_9_clicked()
{
    QMessageBox *b=new QMessageBox("再次确认",		///--这里是设置消息框标题
                                   "您真的要执行该操作吗？",						///--这里是设置消息框显示的内容
                                   QMessageBox::Question,							///--这里是在消息框显示的图标
                                   QMessageBox::Ok | QMessageBox::Default,		///---这里是显示消息框上的按钮情况
                                   QMessageBox::Cancel | QMessageBox::Escape,	///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
                                   0);
    int a=b->exec();
    if(a==QMessageBox::Ok){
        for(int i=0;i<allapply->length();i++){
            if(backapply->at(currentrowback)==allapply->at(i))
            {
                QString t="";
                for(int i=0;i<3;i++)
                {
                    t=t+backapply->at(currentrowback).split(" ").at(i)+" ";
                }
                t=t+"1"+" -1";
                backapply->replace(currentrowback,t);
                allapply->replace(i,t);
                QString a="";
                for(int i=0;i<allapply->length();i++){
                    a=a+allapply->at(i)+"\n";
                }
                QFile f1("message/message.txt");
                f1.open(QIODevice::WriteOnly|QIODevice::Text);
                QTextStream st(&f1);
                st<<a;
                f1.close();
            }
        }
        showback(backapply);
        ui->comboBox_4->setCurrentIndex(0);
    }
    ui->pushButton_9->setEnabled(false);
}

void Sub1::on_pushButton_4_clicked()
{
    QString name = this->ui->lineEdit_2->text();
        if(name==""){
            setConnent(userList,1);
            this->ui->label_5->setText(QString::number(userList->size()));
        }else{
            QStringList* tempList=new QStringList;

            for (int i=0;i<userList->size();i++) {
                QStringList oneUser = userList->at(i).split(" ");
                if(oneUser.at(1).contains(name)){
                    tempList->append(oneUser.join(" "));
                }
            }
            this->ui->label_5->setText(QString::number(tempList->size()));
            setConnent(tempList,1);
            delete tempList;
        }
        setHead(1);
}

void Sub1::on_pushButton_8_clicked()
{
    QString name = this->ui->lineEdit->text();
        if(name==""){
            setConnent(instrumentList,2);
            this->ui->label_7->setText(QString::number(instrumentList->size()));
        }else{
            QStringList* tempList=new QStringList;
            for (int i=0;i<instrumentList->size();i++) {
                QStringList oneInstrument = instrumentList->at(i).split(" ");
                if(oneInstrument.at(1).contains(name)){
                    tempList->append(oneInstrument.join(" "));
                }
            }
            this->ui->label_7->setText(QString::number(tempList->size()));
            setConnent(tempList,2);
            delete tempList;
        }
        this->ui->pushButton->setEnabled(false);
        this->ui->pushButton_2->setEnabled(false);
        this->ui->pushButton_3->setEnabled(false);
        setHead(2);
}

void Sub1::on_tableView_3_clicked(const QModelIndex &index)
{
    int row = index.row();
        QString instrumentId = modelInstrument->data(modelInstrument->index(row,0)).toString();
        int state = checkState(instrumentId);
        if(state==1){
            this->ui->pushButton->setEnabled(true);
            this->ui->pushButton_2->setEnabled(true);
            this->ui->pushButton_3->setEnabled(false);
        }
        if(state==2){
            this->ui->pushButton->setEnabled(false);
            this->ui->pushButton_2->setEnabled(false);
            this->ui->pushButton_3->setEnabled(true);
        }else if(state==3) {
            this->ui->pushButton->setEnabled(false);
            this->ui->pushButton_2->setEnabled(false);
            this->ui->pushButton_3->setEnabled(false);
        }
}

void Sub1::on_pushButton_3_clicked()
{
    QMessageBox w;
        w.setIcon(QMessageBox::Warning);
        w.setText("确认取消移交？");
        w.addButton("确定",QMessageBox::AcceptRole);
        w.addButton("取消",QMessageBox::RejectRole);
        int re=w.exec();
        if(re==QMessageBox::AcceptRole){
           int row = this->ui->tableView_3->currentIndex().row();
           QString instrumentCode = modelInstrument->data(modelInstrument->index(row,0)).toString();
           for (int i=0;i<messageList->size();i++) {
               QStringList tempList = messageList->at(i).split(" ");
               if((tempList.at(0)=="B"||tempList.at(0)=="C")&&tempList.at(2)==instrumentCode){
                   QString messageAddress="message/message.txt";
                   messageList->removeAt(i);
                   writeFile(messageAddress,messageList);
               }
           }
        }
        this->ui->pushButton->setEnabled(true);
        this->ui->pushButton_2->setEnabled(true);
        this->ui->pushButton_3->setEnabled(false);
}

void Sub1::refresh()
{
    if(dialogDepart->getIsRefresh()==true){
            this->ui->pushButton->setEnabled(false);
            this->ui->pushButton_2->setEnabled(false);
            this->ui->pushButton_3->setEnabled(true);
            dialogDepart->setIsRefresh(false);
        }else if(dialogCrossDepart->getIsRefresh()==true){
            this->ui->pushButton->setEnabled(false);
            this->ui->pushButton_2->setEnabled(false);
            this->ui->pushButton_3->setEnabled(false);
            dialogCrossDepart->setIsRefresh(false);
    }
}

void Sub1::on_lineEdit_2_textChanged(const QString &arg1)
{
    this->ui->pushButton_4->click();
}

void Sub1::on_lineEdit_textChanged(const QString &arg1)
{
    this->ui->pushButton_8->click();
}
