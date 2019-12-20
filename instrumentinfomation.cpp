#include "instrumentinfomation.h"
#include "ui_instrumentinfomation.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>
#include <QtDebug>
using namespace QtCharts;
InstrumentInfomation::InstrumentInfomation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InstrumentInfomation)
{
    ui->setupUi(this);
    model=new QStandardItemModel;
    usinglog=new QStringList;
    setheader(6,"时间 事件 部门 经办人 审核人 残值",model);
    this->ui->widget->setStartAngle(0);
    this->ui->widget->setEndAngle(0);
    this->ui->widget->setTitle("残值");
    this->ui->tableView->setModel(model);
}

InstrumentInfomation::~InstrumentInfomation()
{
    delete ui;
    delete model;
    delete usinglog;
}
//传入需要显示的仪器ID
void InstrumentInfomation::setInstrumentId(const QString &value)
{
    this->ui->tableView->setColumnWidth(0,90);
    this->ui->tableView->setColumnWidth(1,45);
    this->ui->tableView->setColumnWidth(2,90);
    this->ui->tableView->setColumnWidth(3,90);
    this->ui->tableView->setColumnWidth(4,90);
    this->ui->tableView->setColumnWidth(5,45);
    instrumentId = value;
    if(!readFile("baseinformation/instrument-using.txt"))
        if(!readFile("baseinformation/instrument-used.txt"))
            readFile("baseinformation/instrument-unused.txt");
    if(!readFile("usinglog/"+instrumentId+".txt",usinglog)){
        QMessageBox::warning(this,"提示","使用记录丢失");
    }else{
        QStringList temp=exchange();
        settable(6,model,&temp);
        createChart(getPoints());
    }
}

bool InstrumentInfomation::readFile(QString path, QStringList *container)
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
//根据传入的编号查找设备信息和计算残值并显示显示信息和残值
bool InstrumentInfomation::readFile(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(line.split(" ").at(0)==instrumentId){
            setInformation(line);
            this->ui->widget->setValue(getCanZhi(QDate::currentDate().toString("yyyy-MM")));
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}
//显示设备基本信息
void InstrumentInfomation::setInformation(QString s)
{
    QStringList temp=s.split(" ");
    this->ui->lab_id->setText(instrumentId);
    this->ui->lb_name->setText(temp.at(1));
    this->ui->lab_number->setText(temp.at(2));
    this->ui->lab_guige->setText(temp.at(3));
    this->ui->lab_price->setText(temp.at(4));
    this->ui->lab_time->setText(temp.at(5)+" "+temp.at(6));
}
//通过时间计算权值
int InstrumentInfomation::getCanZhi(QString end)
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
//创建残值曲线
void InstrumentInfomation::createChart(QList<QDateTime> *data)
{
    QChart *chart= new QChart();
    QChartView *chartView= new QChartView(chart);
    QGridLayout *layout=new QGridLayout();
    QDateTimeAxis *axisX=new QDateTimeAxis;
    QValueAxis *axisY=new QValueAxis;
    QLineSeries *series = new QLineSeries;
    axisX->setFormat("yy/MM");
    axisX->setLabelsAngle(60);
    axisX->setTickCount(10);
    axisX->setRange(data->at(0),data->last());
    axisY->setRange(0,100);
    axisY->setTickCount(10);
    for(auto&i:*data){
        series->append(i.toMSecsSinceEpoch(),getCanZhi(i.toString("yyyy-MM")));
    }
    series->setName("残值曲线");
    chart->addSeries(series);
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    layout->addWidget(chartView,1,0);
    this->ui->wg_graph->setLayout(layout);

}

void InstrumentInfomation::setheader(int column, QString head, QStandardItemModel *model)
{
    for(int i=0;i<column;i++){
        model->setHorizontalHeaderItem(i,new QStandardItem(head.split(" ").at(i)));
    }
}

void InstrumentInfomation::settable(int column, QStandardItemModel *model, QStringList *data)
{
    for (int i=0;i<data->length();i++) {
        for(int j=0;j<column;j++){
            model->setItem(i,j,new QStandardItem(data->at(data->length()-i-1).split(" ").at(j)));
            model->item(i,j)->setTextAlignment(Qt::AlignCenter);
        }
        this->ui->tableView->setRowHeight(i,50);
    }
}
//从使用日志中得到所有时间点
QList<QDateTime> *InstrumentInfomation::getPoints()
{
    QList<QDateTime> * temp=new QList<QDateTime>;
    QDateTime dt;
    QDate d;
    QTime t;
    for(auto&i:*usinglog){
        d.setDate(i.split(" ").at(0).split("-").at(0).toInt(),i.split(" ").at(0).split("-").at(1).toInt(),i.split(" ").at(0).split("-").at(2).toInt());
        t.setHMS(i.split(" ").at(1).split(":").at(0).toInt(),i.split(" ").at(1).split(":").at(1).toInt(),0);
        dt.setDate(d);
        dt.setTime(t);
        temp->append(dt);
    }
    return temp;
}

QString InstrumentInfomation::getDepartmentName(QString id)
{
    QString temp;
    QFile file("baseinformation/department.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "×";
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString departments = in.readLine();
        if(id==departments.split(" ").at(0)){
            temp=departments.split(" ").at(1);
            break;
        }
    }
    file.close();
    return temp;
}

QString InstrumentInfomation::getUserName(QString id)
{
    QString temp;
    QFile file("baseinformation/user.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "×";
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString departments = in.readLine();
        if(id==departments.split(" ").at(0)){
            temp=departments.split(" ").at(1);
            break;
        }
    }
    file.close();
    return temp;
}

QStringList InstrumentInfomation::exchange()
{
    QStringList temp;
    QString t;
    for(auto&i:*usinglog){
        temp.append(i.split(" ").at(0)+"\n"+i.split(" ").at(1)+" "+i.split(" ").at(2)+" "+getDepartmentName(i.split(" ").at(3))+
                    " "+getUserName(i.split(" ").at(4))+" "+getUserName(i.split(" ").at(5))+" "+t.setNum(getCanZhi(i.split(" ").at(0))));
    }
    return temp;
}
