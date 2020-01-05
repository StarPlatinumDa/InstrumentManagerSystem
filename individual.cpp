#include "individual.h"
#include "ui_individual.h"
#include "global.h"
#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>
#include <QMessageBox>
Individual::Individual(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Individual)
{
    datetime=QDateTime::currentDateTime();
    ui->setupUi(this);
    initialize();
    readFile("message/message.txt",messages);
    readFile("baseinformation/instrument-unused.txt",instruments);
//    user="UH100013 张五侠 DE1002 2 e10adc3949ba59abbe56e057f20f883e";
//    this->ui->lab_username->setText(user.split(" ").at(1)+" "+getDepartmentName(user.split(" ").at(2)));
    if(!readFileForInstrument()){
        QMessageBox::warning(this,"错误","系统文件丢失");
    }
    this->ui->tableView_apply->setModel(modelForMessage);
    this->ui->tableView_instrument->setModel(modelForIntrument);
    selectMessage();
    refreshMessage();
    refreshInstrument();
}

Individual::~Individual()
{
    delete ui;
    delete instruments;
    delete instrumentsForIndividual;
    delete messages;
    delete messagesForIndividual;
    delete modelForMessage;
    delete modelForIntrument;
}

void Individual::on_wg_guide_currentItemChanged(int index, const QString &)
{
    int width1=this->ui->tableView_instrument->width();
    int width=this->ui->tableView_apply->width();
    this->ui->tableView_apply->setColumnWidth(0,width/4);
    this->ui->tableView_apply->setColumnWidth(1,width/4);
    this->ui->tableView_apply->setColumnWidth(2,width/4);
    this->ui->tableView_apply->setColumnWidth(3,width/4);
    this->ui->tableView_instrument->setColumnWidth(0,width1/4);
    this->ui->tableView_instrument->setColumnWidth(1,width1/4);
    this->ui->tableView_instrument->setColumnWidth(2,width1/4);
    this->ui->tableView_instrument->setColumnWidth(3,width1/4);
    this->ui->stackedWidget->setCurrentIndex(index);
}


void Individual::on_pb_cancel_clicked()
{
    this->ui->stackedWidget_2->setCurrentIndex(0);
}

void Individual::on_pushButton_3_clicked()
{
    this->ui->stackedWidget_2->setCurrentIndex(1);
}

void Individual::setProgressStyle(QString steps)
{
    this->ui->wg_progreese->setTopInfo(steps.split(" "));
    this->ui->wg_progreese->setMaxStep(steps.split(" ").length());
}

bool Individual::readFile(QString path, QStringList *container)
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

bool Individual::readFileForUser()
{
    Global *temp=Global::getInstant();
    QFile file("baseinformation/user.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        users->append(line);
        if(temp->getUserid()==line.split(" ").at(0)){
            user=line;
        }
    }
    file.close();
    return true;
}

bool Individual::writeFile(QString path, QString data)
{
    QFile file(path);
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return false;
    QTextStream out(&file);
    out<<data<<"\n";
    file.close();
    return true;
}

bool Individual::writeFile1(QString path, QStringList *data)
{
    QFile file(path);
    if (!file.open(QIODevice::Truncate | QIODevice::Text))
        return false;
    QTextStream out(&file);
   for(int i=0;i<data->length();i++){
       out<<data->at(i)<<"\n";
   }
    file.close();
    return true;
}

bool Individual::readFileForInstrument()
{
    QFile file("baseinformation/instrument-using.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        instruments->append(line);
        if(user.split(" ").at(0)==line.right(8)){
            instrumentsForIndividual->append(line);
        }
    }
    file.close();
    this->ui->lab_instrumentnumber->setNum(instrumentsForIndividual->length());
    return true;
}

void Individual::setheader(int column, QString head, QStandardItemModel *model)
{
    for(int i=0;i<column;i++){
        model->setHorizontalHeaderItem(i,new QStandardItem(head.split(" ").at(i)));
    }
}

void Individual::settable(int column, QStandardItemModel *model, QStringList *data)
{
    for (int i=0;i<data->length();i++) {
        for(int j=0;j<column;j++){
            model->setItem(i,j,new QStandardItem(data->at(i).split(" ").at(j)));
            model->item(i,j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void Individual::selectMessage()
{
    for (QString i:*messages) {
        if(i.contains(user.left(8))){
            messagesForIndividual->append(i);
        }
    }
    this->ui->lab_messagenumber->setNum(messagesForIndividual->length());
}

QString Individual::getDepartmentName(QString id)
{
    QStringList departments;
    readFile("baseinformation/department.txt",&departments);
    QString temp;
    for(int i=0;i<departments.length();i++){
        if(id==departments.at(i).split(" ").at(0)){
            temp=departments.at(i).split(" ").at(1);
            break;
        }
    }
    return temp;
}

QString Individual::getUserDepartmentId(QString id)
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

QStringList *Individual::getMessageData()
{
    QStringList *data=new QStringList;
    for(QString i:*messagesForIndividual){
        if(i.left(1)=="D"||i.left(1)=="C"||i.left(1)=="B"){
            QString temp;
            if(i.left(1)=="D")
                temp="归还设备";
            else if(i.left(1)=="B")
                temp="内部移交";
            else
                temp="跨部门移交";
            temp+=getInstrumentinformation("baseinformation/instrument-using.txt",i.split(" ").at(2));
            temp+=" "+state(i).split(" ").at(0);
            data->append(temp);
        }else{
            QString temp="申请设备";
            temp+=getInstrumentinformation("baseinformation/instrument-unused.txt",i.split(" ").at(2));
            temp+=" "+state(i).split(" ").at(0);
            data->append(temp);
        }
    }
    return data;
}

QString Individual::state(QString message)
{
    QString states;
    if(message.left(1)=="A"||message.left(1)=="D"){
        QString a=message.split(" ").at(3);
        QString b=message.split(" ").at(4);
        if(a=="-1")
            states= "待部门审核 1 -1";
        else if(a=="0")
            states= "部门审核未通过 2 0";
        else{
            if(b=="-1")
                states= "待国资审核 2 -1";
            else if(b=="0")
                states= "国资审核未通过 3 0";
            else
                states= "通过 4 1";
        }
    }else if(message.left(1)=="B"){
        QString a=message.split(" ").at(4);
        QString b=message.split(" ").at(6);
        if(a=="-1")
            states= "待移交 1 -1";
        else{
            if(b=="-1")
                states= "待接收 2 -1";
            else
                states= "完成 4 1";
        }
    }else{
        QString c=message.split(" ").at(3);
        QString a=message.split(" ").at(5);
        QString b=message.split(" ").at(7);
        if(c=="-1")
            states= "待国资审核 1 -1";
        else if(c=="0")
            states= "国资审核未通过 2 0";
        else{
            if(a=="-1")
                states= "待移交 2 -1";
            else{
                if(b=="-1")
                    states= "待接收 3 -1";
                else
                    states= "完成 5 1";
            }
        }

    }
    return states;
}

QString Individual::getInstrumentinformation(QString path,QString id)
{
    QString temp;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return nullptr;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(line.split(" ").at(0)==id){
            temp=" "+line.split(" ").at(1)+" "+id;
        }
    }
    file.close();
    return temp;
}

void Individual::setCurrentState(QString s)
{
    this->ui->wg_progreese->setCurrentStep(s.split(" ").at(1).toInt());
    if(s.split(" ").at(2).toInt()==0)
        this->ui->wg_progreese->setCurrentBackground(QColor("red"));
    else
        this->ui->wg_progreese->setCurrentBackground(color);
}

void Individual::refreshMessage()
{
    modelForMessage->clear();
    setheader(4,"类型 设备名称 设备条码 当前状态",modelForMessage);
    settable(4,modelForMessage,getMessageData());
    this->ui->tableView_apply->setColumnWidth(0,160);
    this->ui->tableView_apply->setColumnWidth(1,160);
    this->ui->tableView_apply->setColumnWidth(2,180);
    this->ui->tableView_apply->setColumnWidth(3,160);
}

void Individual::refreshInstrument()
{
    setheader(4,"设备名称 设备条码 型号 价格",modelForIntrument);
    settable(4,modelForIntrument,getInstrumentData());
    this->ui->tableView_instrument->setColumnWidth(0,160);
    this->ui->tableView_instrument->setColumnWidth(1,160);
    this->ui->tableView_instrument->setColumnWidth(2,180);
    this->ui->tableView_instrument->setColumnWidth(3,160);
}

QStringList *Individual::getInstrumentData()
{
    QStringList *temp=new QStringList;
    for(QString i:*instrumentsForIndividual){
        temp->append(i.split(" ").at(1)+" "+i.split(" ").at(0)+" "+i.split(" ").at(3)+" "+i.split(" ").at(4));
    }
    return temp;
}

int Individual::getCanZhi(QString end)
{
    QString temp=this->ui->lab_time->text().split(" ").at(0);
    QDate in(temp.split("-").at(0).toInt(),temp.split("-").at(1).toInt(),1);
    QDate toend(end.split("-").at(0).toInt(),end.split("-").at(1).toInt(),1);
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

void Individual::setInstrumentInformation(QString s)
{
    QStringList temp=s.split(" ");
    this->ui->lab_id->setText(temp.at(0));
    this->ui->lab_name->setText(temp.at(1));
    this->ui->lab_bianhao->setText(temp.at(2));
    this->ui->lab_mode->setText(temp.at(3));
    this->ui->lab_price->setText(temp.at(4));
    this->ui->lab_time->setText(temp.at(5)+" "+temp.at(6));
    this->ui->wg_canzhi->setValue(getCanZhi(QDate::currentDate().toString("yyyy-MM")));
}

void Individual::on_checkBox_stateChanged(int arg1)
{
    if(arg1){
        this->ui->le_password_new->setEchoMode(QLineEdit::Normal);
        this->ui->le_password_old->setEchoMode(QLineEdit::Normal);
    }else{
        this->ui->le_password_new->setEchoMode(QLineEdit::Password);
        this->ui->le_password_old->setEchoMode(QLineEdit::Password);
    }
}

void Individual::initialize()
{
    this->ui->wg_guide->setBarStyle(NavBar::BarStyle_Line_Right);
    this->ui->wg_guide->setHorizontal(false);
    this->ui->wg_guide->setFlat(true);
    this->ui->wg_guide->setItems("个人中心|设备管理");
    this->ui->wg_guide->setBarColorStart(QColor("gray"));
    this->ui->wg_guide->setBarColorEnd(QColor("gray"));
    this->ui->wg_guide->setBgColorStart(QColor("white"));
    this->ui->wg_guide->setBgColorEnd(QColor("white"));
    this->ui->wg_guide->setTextNormalColor(QColor("black"));
    this->ui->wg_guide->setTextSelectColor(QColor("orange"));
    this->ui->widget->setBorderColor(QColor("orange"));
    this->ui->wg_canzhi->setRange(0,100);
    this->ui->wg_canzhi->setPercentStyle(ProgressPercent::PercentStyle_Arc_Wave);
    this->ui->wg_canzhi->setWaterDensity(4);
    this->ui->wg_canzhi->setWaterHeight(3);
    this->ui->wg_canzhi->setShowSmallCircle(true);
    this->ui->wg_canzhi->setNullPosition(-90);
    color=this->ui->wg_progreese->getCurrentBackground();
    modelnumber=1;
    setProgressStyle("- - - -");
    instrumentsForIndividual=new QStringList;
    instruments=new QStringList;
    messages=new QStringList;
    messagesForIndividual=new QStringList;
    modelForMessage=new QStandardItemModel;
    modelForIntrument=new QStandardItemModel;
    modelForUnused=new QStandardItemModel;
    users=new QStringList;
    readFileForUser();
    this->ui->lab_username->setText(user.split(" ").at(1)+" "+getDepartmentName(user.split(" ").at(2)));
}

void Individual::on_tableView_apply_clicked(const QModelIndex &index)
{
    instrumentId=modelForMessage->item(index.row(),2)->text();
    for(QString i:*messagesForIndividual){
        if(i.contains(instrumentId)){
            instrumentId=i;
            break;
        }
    }
    if(instrumentId.left(1)=="A"||instrumentId.left(1)=="D"){
        setProgressStyle("提交申请 部门审核 国资审核 完成");
        setCurrentState(state(instrumentId));
        if(this->ui->wg_progreese->getCurrentStep()<2){
            this->ui->pb_diffrent->setEnabled(true);
            this->ui->pb_diffrent->setText("撤销申请");
        }else if(this->ui->wg_progreese->getCurrentStep()>3||state(instrumentId).split(" ").at(2)=="0"){
            this->ui->pb_diffrent->setEnabled(true);
            this->ui->pb_diffrent->setText("删除记录");
        }else {
            this->ui->pb_diffrent->setEnabled(false);
            this->ui->pb_diffrent->setText("删除记录");
        }
    }else if(instrumentId.left(1)=="B"){
        setProgressStyle("移交建立 移交 接收 完成");
        setCurrentState(state(instrumentId));
        if(instrumentId.split(" ").at(3)==user.split(" ").at(0)){
            if(this->ui->wg_progreese->getCurrentStep()==1){
                this->ui->pb_diffrent->setEnabled(true);
                this->ui->pb_diffrent->setText("移交");
            }else if(this->ui->wg_progreese->getCurrentStep()>2){
                this->ui->pb_diffrent->setEnabled(true);
                this->ui->pb_diffrent->setText("删除记录");
            }else {
                this->ui->pb_diffrent->setEnabled(false);
                this->ui->pb_diffrent->setText("删除记录");
            }
        }else{
            if(this->ui->wg_progreese->getCurrentStep()==2){
                this->ui->pb_diffrent->setEnabled(true);
                this->ui->pb_diffrent->setText("接收");
            }else if(this->ui->wg_progreese->getCurrentStep()>2){
                this->ui->pb_diffrent->setEnabled(true);
                this->ui->pb_diffrent->setText("删除记录");
            }else {
                this->ui->pb_diffrent->setEnabled(false);
                this->ui->pb_diffrent->setText("删除记录");
            }
        }
    }else{
        setProgressStyle("移交建立 国资审核 移交 接收 完成");
        setCurrentState(state(instrumentId));
        if(instrumentId.split(" ").at(4)==user.split(" ").at(0)){
            if(state(instrumentId).split(" ").at(1)=="2"&&state(instrumentId).split(" ").at(2)!="0"){
                this->ui->pb_diffrent->setEnabled(true);
                this->ui->pb_diffrent->setText("移交");
            }else if(this->ui->wg_progreese->getCurrentStep()>4){
                this->ui->pb_diffrent->setEnabled(true);
                this->ui->pb_diffrent->setText("删除记录");
            }else {
                this->ui->pb_diffrent->setEnabled(false);
                this->ui->pb_diffrent->setText("删除记录");
            }
        }else{
            if(this->ui->wg_progreese->getCurrentStep()==3){
                this->ui->pb_diffrent->setEnabled(true);
                this->ui->pb_diffrent->setText("接收");
            }else if(this->ui->wg_progreese->getCurrentStep()>4){
                this->ui->pb_diffrent->setEnabled(true);
                this->ui->pb_diffrent->setText("删除记录");
            }else {
                this->ui->pb_diffrent->setEnabled(false);
                this->ui->pb_diffrent->setText("删除记录");
            }
        }
    }

}

void Individual::on_tableView_instrument_clicked(const QModelIndex &index)
{
    QString id;
    if(modelnumber==1){
        id=modelForIntrument->item(index.row(),1)->text();
        ui->pb_return->setEnabled(true);
    }else{
        id=modelForUnused->item(index.row(),1)->text();
        ui->pb_apply->setEnabled(true);
    }
    for(QString i:*instruments){
        if(i.contains(id)){
            id=i;
            break;
        }
    }
    setInstrumentInformation(id);
}
void Individual::resizeEvent(QResizeEvent *){
    QDateTime temp=QDateTime::currentDateTime();
    if(datetime.addSecs(5)<temp){

        int width1=this->ui->tableView_instrument->width();
        int width=this->ui->tableView_apply->width();
        this->ui->tableView_apply->setColumnWidth(0,width/4);
        this->ui->tableView_apply->setColumnWidth(1,width/4);
        this->ui->tableView_apply->setColumnWidth(2,width/4);
        this->ui->tableView_apply->setColumnWidth(3,width/4);
        this->ui->tableView_instrument->setColumnWidth(0,width1/4);
        this->ui->tableView_instrument->setColumnWidth(1,width1/4);
        this->ui->tableView_instrument->setColumnWidth(2,width1/4);
        this->ui->tableView_instrument->setColumnWidth(3,width1/4);
    }
}

QString Individual::encode(QString &s)
{
    QByteArray ps;
    ps.append(s);
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(ps);
    QByteArray result=hash.result();
    QString md5=result.toHex();
    return md5;
}

void Individual::refreshApply()
{//名称 条码 型号 入库时间
    QStringList *unused=new QStringList;
    QStringList *data=new QStringList;
    readFile("baseinformation/instrument-unused.txt",unused);
    for(auto&i:*unused){
        bool have=false;
        for(auto&j:*messages){
            if(j.split(" ").at(2)==i.split(" ").at(0)){
                have=true;
                break;
            }
        }
        if(!have){
            data->append(i.split(" ").at(1)+" "+i.split(" ").at(0)+" "+i.split(" ").at(3)+" "+i.split(" ").at(5));
        }
    }
    setheader(4,"名称 条码 型号 入库时间",modelForUnused);
    settable(4,modelForUnused,data);
    ui->tableView_instrument->setModel(modelForUnused);
    int width1=this->ui->tableView_instrument->width();
    this->ui->tableView_instrument->setColumnWidth(0,width1/4);
    this->ui->tableView_instrument->setColumnWidth(1,width1/4);
    this->ui->tableView_instrument->setColumnWidth(2,width1/4);
    this->ui->tableView_instrument->setColumnWidth(3,width1/4);
}


void Individual::on_pb_ok_clicked()
{
    QString old=this->ui->le_password_old->text();
    if(user.split(" ").at(4)==encode(old)){
        QString now=this->ui->le_password_new->text();
        if(now!=""){
            int place=users->indexOf(user);
            user.replace(encode(old),encode(now));
            users->replace(place,user);
            QFile file("baseinformation/user.txt");
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
            QTextStream out(&file);
            for(QString i:*users){
                out<<i<<"\n";
            }
            file.close();
            QMessageBox::information(this,"成功","修改密码成功");
            this->ui->le_password_new->clear();
            this->ui->le_password_old->clear();
            this->ui->pb_cancel->click();
        }else{
            QMessageBox::warning(this,"提示","请输入新密码");
        }
    }else{
        QMessageBox::critical(this,"错误","密码错误");
    }
}

void Individual::on_pb_apply_clicked()
{
    if(ui->pb_apply->text()=="申请设备"){
        refreshApply();
        modelnumber=2;
        ui->pb_apply->setText("申请");
        ui->pb_return->setText("返回");
        ui->pb_apply->setEnabled(false);
        ui->pb_return->setEnabled(true);
    }else{
        QMessageBox w;
        w.addButton("确定",QMessageBox::AcceptRole);
        w.addButton("取消",QMessageBox::RejectRole);
        w.setIcon(QMessageBox::Information);
        w.setWindowTitle("申请");
        w.setText("确定提交申请？");
        int re=w.exec();
        if(re==QMessageBox::AcceptRole){
            QString temp="A "+user.split(" ").at(0)+" "+ui->lab_id->text()+" -1 -1";
            messages->append(temp);
            messagesForIndividual->append(temp);
            refreshMessage();
            writeFile("message/message.txt",temp);
            refreshApply();
        }
        ui->pb_apply->setEnabled(false);
    }
}

void Individual::on_pb_return_clicked()
{
    if(ui->pb_return->text()=="返回"){
        ui->tableView_instrument->setModel(modelForIntrument);
        modelnumber=1;
        ui->pb_return->setText("归还设备");
        ui->pb_apply->setEnabled(true);
        ui->pb_apply->setText("申请设备");
        ui->pb_return->setEnabled(false);
    }else{
        QMessageBox w;
        w.addButton("确定",QMessageBox::AcceptRole);
        w.addButton("取消",QMessageBox::RejectRole);
        w.setIcon(QMessageBox::Information);
        w.setWindowTitle("归还");
        w.setText("确定归还设备？");
        int re=w.exec();
        if(re==QMessageBox::AcceptRole){
            bool have=false;
            for(auto&i:*messagesForIndividual){
                if(i.contains(ui->lab_id->text())){
                    QMessageBox::critical(this,"错误","设备已申请归还");
                    have=true;
                }
            }
            if(!have){
                QString temp="D "+user.split(" ").at(0)+" "+ui->lab_id->text()+" -1 -1";
                messages->append(temp);
                messagesForIndividual->append(temp);
                refreshMessage();
                writeFile("message/message.txt",temp);
            }
        }
        ui->pb_return->setEnabled(false);
    }
}

void Individual::on_pb_diffrent_clicked()
{
    int re=-10;
    if(ui->pb_diffrent->text()=="删除记录"||ui->pb_diffrent->text()=="撤销申请"){
        QMessageBox w;
        w.addButton("确定",QMessageBox::AcceptRole);
        w.addButton("取消",QMessageBox::RejectRole);
        w.setIcon(QMessageBox::Information);
        w.setWindowTitle("提示");
        w.setText("确定永久删除该记录？");
        re=w.exec();
        if(re==QMessageBox::AcceptRole){
            for(auto&i:*messages){
                if(i.contains(instrumentId)){
                    messages->removeOne(i);
                }
            }
        }
    }else if(ui->pb_diffrent->text()=="移交"){
        if(ui->wg_progreese->getMaxStep()==4){
            for(auto&i:*messages){
                if(i.contains(instrumentId)){
                    i=i.split(" ").at(0)+" "+i.split(" ").at(1)+" "+i.split(" ").at(2)+" "+i.split(" ").at(3)+" 1 "+
                            i.split(" ").at(5)+" "+i.split(" ").at(6);
                    re=QMessageBox::AcceptRole;
                    break;
                }
            }
        }else{
            for(auto&i:*messages){
                if(i.contains(instrumentId)){
                    i=i.split(" ").at(0)+" "+i.split(" ").at(1)+" "+i.split(" ").at(2)+" "+i.split(" ").at(3)+" "+
                            i.split(" ").at(4)+" 1 "+i.split(" ").at(6)+" "+i.split(" ").at(7);
                    re=QMessageBox::AcceptRole;
                    break;
                }
            }
        }
        QMessageBox::information(this,"成功","已移交");
    }else{
        if(ui->wg_progreese->getMaxStep()==4){
            for(auto&i:*messages){
                if(i.contains(instrumentId)){
                    i=i.split(" ").at(0)+" "+i.split(" ").at(1)+" "+i.split(" ").at(2)+" "+i.split(" ").at(3)+" "+
                            i.split(" ").at(4)+" "+i.split(" ").at(5)+" 1";
                    re=QMessageBox::AcceptRole;
                    QFile file("usinglog/"+instrumentId.split(" ").at(2)+".txt");
                    if (!file.open(QIODevice::Append | QIODevice::Text))
                        return;
                    QTextStream out(&file);
                    out<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")+" 移交 "+getUserDepartmentId(i.split(" ").at(5))+" "
                         +i.split(" ").at(5)+" "+i.split(" ").at(1);
                    file.close();
                    for(auto&j:*instruments){
                        if(j.contains(instrumentId.split(" ").at(2))){
                            j=j.replace(j.split(" ").at(7),i.split(" ").at(5));
                            break;
                        }
                    }
                    writeFile1("baseinformation/instrument-using.txt",instruments);
                    break;
                }
            }
        }else{
            for(auto&i:*messages){
                if(i.contains(instrumentId)){
                    i=i.split(" ").at(0)+" "+i.split(" ").at(1)+" "+i.split(" ").at(2)+" "+i.split(" ").at(3)+" "+
                            i.split(" ").at(4)+" "+i.split(" ").at(5)+" "+i.split(" ").at(6)+" 1 "+i.split(" ").at(8);
                    re=QMessageBox::AcceptRole;
                    QFile file("usinglog/"+instrumentId.split(" ").at(2)+".txt");
                    if (!file.open(QIODevice::Append | QIODevice::Text))
                        return;
                    QTextStream out(&file);
                    out<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")+" 移交 "+getUserDepartmentId(i.split(" ").at(6))+" "
                         +i.split(" ").at(6)+" "+i.split(" ").at(8);
                    file.close();
                    for(auto&j:*instruments){
                        if(j.contains(instrumentId.split(" ").at(2))){
                            j=j.replace(j.split(" ").at(7),i.split(" ").at(6));
                            break;
                        }
                    }
                    writeFile1("baseinformation/instrument-using.txt",instruments);
                    break;
                }
            }
        }
        QMessageBox::information(this,"成功","接收成功");
    }
    if(re==QMessageBox::AcceptRole){
        messagesForIndividual->clear();
        selectMessage();
        refreshMessage();
        QFile file("message/message.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        for(auto&i:*messages){
            out<<i<<"\n";
        }
        file.close();
    }
    ui->pb_diffrent->setEnabled(false);
}
