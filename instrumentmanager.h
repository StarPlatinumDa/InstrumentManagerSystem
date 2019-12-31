#ifndef INSTRUMENTMANAGER_H
#define INSTRUMENTMANAGER_H

#include <QDialog>
#include <QStringList>
#include <QStandardItemModel>
namespace Ui {
class InstrumentManager;
}

class InstrumentManager : public QDialog
{
    Q_OBJECT

public:
    explicit InstrumentManager(QWidget *parent = nullptr);
    ~InstrumentManager();

private slots:
    void on_pushButton_3_clicked();

    void on_pb_search_1_clicked();

    void on_pb_add_clicked();

    void addnew(QString name);

    void on_cb_department_name_currentTextChanged(const QString &arg1);

    void on_tabWidget_currentChanged(int index);

    void on_pb_search_3_clicked();

    void on_le_instrument_name_textChanged(const QString);

    void on_le_user_name_textChanged(const QString);

    void on_tableView_3_doubleClicked(const QModelIndex &index);

    void on_tableView_1_doubleClicked(const QModelIndex &index);

    void on_tableView_2_doubleClicked(const QModelIndex &index);

    void on_pb_search_4_clicked();

    void on_le_instrument_name_2_textChanged(const QString &);

    void on_cb_shenheren_currentIndexChanged(int);

    void on_tableView_4_doubleClicked(const QModelIndex &index);

    void on_tableView_5_doubleClicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_tableView_5_clicked(const QModelIndex &index);

private:
    Ui::InstrumentManager *ui;
    QStringList *useing;
    QStringList *users;
    QStringList *departments;
    QStringList *unused;
    QStringList *used;
    QStandardItemModel *modelfordepartment;
    QStandardItemModel *modelforcangku;
    QStandardItemModel *modelforuser;
    QStandardItemModel *modelforused;
    QStandardItemModel *modelforbaofei;
    QString baofeiid;
    bool readFile(QString path,QStringList *container);
    void setheader(int column,QString head,QStandardItemModel *model);
    bool writeFile(QString path,QStringList *container);
    void settable(int column,QStandardItemModel *model,QStringList *data);
    QString getDepartmentName(QString id);
    QString getUserName(QString id);
    QString getUserDepartmentId(QString id);
    QString departmentNameToId(QString name);
    int instrumentNumber(QStringList * list,QString NMP);
    QStringList findForUnused();
    QStringList findForDepartment();
    QStringList findForUser(QString name);
    QStringList findForUsed(QString name);
    int getCanZhi(QString start);
    QStringList findForBaofei();
    void refreshBaofei();
};

#endif // INSTRUMENTMANAGER_H
