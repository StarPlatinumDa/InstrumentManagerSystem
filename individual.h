#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <QMainWindow>
#include <QStringList>
#include <QStandardItemModel>
#include <QDateTime>
namespace Ui {
class Individual;
}

class Individual : public QMainWindow
{
    Q_OBJECT

public:
    explicit Individual(QWidget *parent = nullptr);
    ~Individual();


private slots:
    void on_wg_guide_currentItemChanged(int index, const QString &);

    void on_pb_cancel_clicked();

    void on_pushButton_3_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_tableView_apply_clicked(const QModelIndex &index);

    void on_tableView_instrument_clicked(const QModelIndex &index);

    void on_pb_ok_clicked();

    void on_pb_apply_clicked();

    void on_pb_return_clicked();

    void on_pb_diffrent_clicked();

private:
    Ui::Individual *ui;
    QString user;
    QStringList *users;
    QStringList *instrumentsForIndividual;
    QStringList *instruments;
    QStringList *messages;
    QStringList *messagesForIndividual;
    QStandardItemModel *modelForMessage;
    QStandardItemModel *modelForIntrument;
    QStandardItemModel *modelForUnused;
    QString instrumentId;
    int modelnumber;
    QDateTime datetime;
    void initialize();
    void setProgressStyle(QString steps);
    bool readFile(QString path,QStringList *container);
    bool readFileForUser();
    bool writeFile(QString path,QString data);
    bool readFileForInstrument();
    void setheader(int column,QString head,QStandardItemModel *model);
    void settable(int column,QStandardItemModel *model,QStringList *data);
    void selectMessage();
    QString getDepartmentName(QString id);
    QStringList *getMessageData();
    QString state(QString message);
    QString getInstrumentinformation(QString path,QString id);
    void setCurrentState(QString s);
    void refreshMessage();
    QColor color;
    void refreshInstrument();
    QStringList *getInstrumentData();
    int getCanZhi(QString end);
    void setInstrumentInformation(QString s);
    void resizeEvent(QResizeEvent *);
    QString encode(QString &s);
    void refreshApply();
};

#endif // INDIVIDUAL_H
