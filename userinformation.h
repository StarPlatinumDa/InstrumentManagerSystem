#ifndef USERINFORMATION_H
#define USERINFORMATION_H

#include <QDialog>
#include <QString>

namespace Ui {
class UserInformation;
}

class UserInformation : public QDialog
{
    Q_OBJECT

public:
    explicit UserInformation(QWidget *parent = nullptr);

    bool isRepeatedUserName(QString& userName);

    bool getDepartIdByDepartName(const QString& name,QString& num);

    bool encode(const QString &s,QString& p);

    bool getPowerNum(const QString &power,QString& num);

    ~UserInformation();

    int getType() const;

    void setType(int value);

    QStringList *getAllUser() const;

    void setAllUser(QStringList *value);

    bool readFile(const QString& fileAddress,QStringList *list); //读取内容放到list，下同

    bool writeFile(const QString& fileAddress,QStringList *list);


    QStringList *getAllDepartMent() const;

    void setAllDepartMent(QStringList *value);

    void setDepartmentBox();//根据文件内容设置部门下拉框内容

    void setConnent(const QString& userName,const QString& name,const QString& department,const QString& power);//设置界面展示内容

    int localSiteInUserList(const QString& username); //返回该账号在用户表中的位置
signals:
    //刷新用户表界面
    void refreshSignal();

private slots:

    void on_pb_cancel_clicked();

    void on_pb_ok_clicked();

    void on_pb_reset_clicked();

private:
    Ui::UserInformation *ui;

    int type; //1代表添加窗口 2代表修改窗口

    QStringList* allUser;//指向父类的用户列表

    QStringList* allDepartMent;//指向父类的部门列表

    QString iniPassword;//初始密码
};

#endif // USERINFORMATION_H
