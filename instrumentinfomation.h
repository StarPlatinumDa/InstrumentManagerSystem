#ifndef INSTRUMENTINFOMATION_H
#define INSTRUMENTINFOMATION_H

#include <QDialog>
#include <QStringList>
#include <QChartView>
#include <QtCharts>
#include <QSplineSeries>
#include <QStandardItemModel>
#include <QList>
namespace Ui {
class InstrumentInfomation;
}

class InstrumentInfomation : public QDialog
{
    Q_OBJECT

public:
    explicit InstrumentInfomation(QWidget *parent = nullptr);
    ~InstrumentInfomation();

    void setInstrumentId(const QString &value);


private:
    Ui::InstrumentInfomation *ui;
    QString instrumentId;
    QStringList *usinglog;
    QStandardItemModel *model;
    bool readFile(QString path,QStringList *container);
    bool readFile(QString path);
    void setInformation(QString s);
    int getCanZhi(QString end);
    void createChart(QList<QDateTime> *data);
    void setheader(int column,QString head,QStandardItemModel *model);
    void settable(int column,QStandardItemModel *model,QStringList *data);
    QList<QDateTime> *getPoints();
    QString getDepartmentName(QString id);
    QString getUserName(QString id);
    QStringList exchange();
};

#endif // INSTRUMENTINFOMATION_H
