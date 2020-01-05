#ifndef SUB1_H
#define SUB1_H

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include "dialog.h"

namespace Ui {
class Sub1;
}

class Sub1 : public QDialog
{
    Q_OBJECT

public:
    explicit Sub1(QWidget *parent = nullptr);
    ~Sub1();
    void readfile(QString filename,QStringList *str);//读文件
    QStringList *backapply;//设备归还申请
    QStringList *useapply;//设备使用申请
    QStringList *allapply;//读取文件中所有信息
    QStringList *thisapply;//分离出本部门的所有信息
    QStringList *dep;//读取所有部门信息
    QString finddepartbyid(QString id);//通过用户id找到他的部门
    void headuse();//使用表头
    void headback();//归还表头
    void showuse(QStringList *str);
    void showback(QStringList *str);
    QStandardItemModel *modeluse;//数据模型
    QStandardItemModel *modelback;//数据模型
    QString findnamebyid(QString id);//通过用户id找到名字
    QString findidbyname(QString name);//通过用户名字找到id
    QString findusenamebyequipid(QString id);//通过设备编码找到名称(未用的设备)
    QString findbacknamebyequipid(QString id);//通过设备编码找到名称(使用中用的设备)
    void searchuse(QString str);
    void searchback(QString str);
    int currentrowuse;//记录现在点的行数
    int currentrowback;//记录现在点的行数

    //secondpart
    bool readFile(const QString& fileAddress,QStringList *list); //读取内容放到list

    bool writeFile(const QString& fileAddress,QStringList *list);

    void setHead(int category); //设置表头

    void setConnent(QStringList* connent,int category);//设置表格内容 1代表用户表 2代表仪器表

    void getLoginDepart(QString& userName , QString& departNum);

    bool isBelongToDepart(QString& oneInfo);//通过using文件中的一条记录中的用户编号判断其是不是该部门的设备

    void getNameById(QString& name , QString& id);

    int checkState(QString instrumentId); //通过设备编号检查当前设备的状态 1代表正常使用中 2代表部门内移交 3代表跨部门移交




private slots:
    void on_pb_user_clicked();

    void on_pb_esearch_clicked();

    void on_pb_emanage_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_comboBox_3_currentTextChanged(const QString &arg1);

    void on_tableView_4_clicked(const QModelIndex &index);

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_comboBox_4_currentTextChanged(const QString &arg1);

    void on_tableView_5_clicked(const QModelIndex &index);

    void on_pushButton_9_clicked();


    //sencond
    void on_pushButton_4_clicked();

    void on_pushButton_8_clicked();

    void on_tableView_3_clicked(const QModelIndex &index);

    void on_pushButton_3_clicked();

    void refresh();

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_textChanged(const QString &arg1);
private:
    Ui::Sub1 *ui;
    //second
    QStringList* departMember;//存储部门用户管理的信息

    QStringList* usingList;//存储正在使用仪器信息

    QStringList* userList;//存储用户信息 格式：用户编号+用户名+拥有设备数量

    QStringList* instrumentList;//存储仪器列表的信息 设备名称+设备编号+型号+价格+当前使用人

    QStringList* allUser;

    QStringList* messageList;

    QStandardItemModel *modelUser;

    QStandardItemModel *modelInstrument;

    Dialog* dialogDepart;//部门内移交窗口

    Dialog* dialogCrossDepart;//跨部门移交窗口
};

#endif // SUB1_H
