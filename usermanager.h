#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QDialog>
#include <QStandardItemModel>
#include "userinformation.h"

namespace Ui {
class UserManager;
}

class UserManager : public QDialog
{
    Q_OBJECT

public:
    explicit UserManager(QWidget *parent = nullptr);

    ~UserManager();

    void setHead(); //设置表头

    bool getDepartByDepartId(const QString& departId,QString& departName);

    bool getPowerByNum(const QString& num,QString& power);

    bool readFile(const QString& fileAddress,QStringList *list); //读取内容放到list，下同

    bool writeFile(const QString& fileAddress,QStringList *list);

    void setConnent(QStringList* connent);//设置表格内容 connent每一条对应user文件里一条用户信息

    int localSiteInUserList(const QString& username); //返回该账号在用户表中的位置

    QStringList *getUserList() const;

    QStringList *getDepartmentList() const;

    bool showSlectUser(const QString& department,const QString& power);//在view界面打印出符合选择框条件的用户,并关闭删除修改按钮

private slots:
    void on_pushButton_clicked();

    void on_pb_add_clicked();

    void refresh();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pb_modify_clicked();

    void on_pb_delete_clicked();

    void on_cb_department_activated(const QString &arg1);

    void on_cb_power_activated(const QString &arg1);


private:
    Ui::UserManager *ui;
    QStandardItemModel *model;
    UserInformation *userinformationAdd; //指向添加用户的窗口
    UserInformation *userinformationModify;  //指向修改用户的窗口
    QStringList *departmentList;  //指向从文件读取的部门信息 每一个是一条
    QStringList *userList; //指向从文件读取的用户信息 每一个是一条
    void resizeEvent(QResizeEvent*);
};


#endif // USERMANAGER_H
