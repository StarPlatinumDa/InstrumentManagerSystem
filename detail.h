#ifndef DETAIL_H
#define DETAIL_H

#include <QDialog>
#include <QStringList>
#include <QStandardItemModel>
namespace Ui {
class Detail;
}

class Detail : public QDialog
{
    Q_OBJECT

public:
    explicit Detail(QWidget *parent = nullptr);
    ~Detail();

    void setStandard(const QString &value);

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::Detail *ui;
    QStandardItemModel *model;
    QStringList *instruments;
    QStringList *users;
    QString standard;
    QString getUserName(QString id);
    void setTableWidth(int &type);
    bool readFile(QString path,QStringList *container);
    void setheader(int column,QString head,QStandardItemModel *model);
    void settable(int column,QStandardItemModel *model,QStringList *data);
    QStringList *instrument(QStringList * list,QString NMP);
    QString getUserDepartmentId(QString id);
    void showInstruments();
};
#endif // DETAIL_H
