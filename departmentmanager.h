#ifndef DEPARTMENTMANAGER_H
#define DEPARTMENTMANAGER_H

#include <QDialog>
#include<department.h>
#include <QStandardItem>
#include <QStandardItemModel>
#include "modify.h"
namespace Ui {
class DepartmentManager;
}

class DepartmentManager : public QDialog
{
    Q_OBJECT

public:
    explicit DepartmentManager(QWidget *parent = nullptr);
    ~DepartmentManager();
    //void tablegenerate(QString s);
    QStandardItemModel *model;//数据模型
    void head();//模型头
    void show(QStringList *str);//表身
    //void searchtwo(QString d,QString pc);//根据选中的部门和负责人进行显示
    void searchdepart(QString d);//根据部门搜索
    void searchfuzeren(QString d);//根据负责人搜索
    QStringList *str;//读入部门文件
    QStringList *fuzeren;//读入负责人文件
    QString searchfuzerenB(QString  f);//寻找负责人
    Modify m;

private slots:

    void on_cb_department_currentTextChanged(const QString &arg1);

    void on_pb_add_clicked();

    void dealnextadd();//添加的槽函数

    void on_tableView_clicked(const QModelIndex &index);

    void on_pb_modify_clicked();

    void on_cb_master_currentTextChanged(const QString &arg1);

private:
    Ui::DepartmentManager *ui;
};

#endif // DEPARTMENTMANAGER_H
