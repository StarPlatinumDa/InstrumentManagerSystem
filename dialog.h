#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "global.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void setSomeEnable(bool choice);//设置部门框是否可见

    void setAtributes(QStringList* message,QStringList* user,QString instrumentCode);//把sub1的messageList、userList传过来

    void empty();

    void getIdByName(QString& name,QString& id);

    bool writeFile(const QString& fileAddress,QStringList *list);

    bool getIsRefresh() const;

    void setIsRefresh(bool value);

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

signals:
    //刷新sub界面 部门内移交 跨部门移交 按钮
    void refreshSignal();

private:
    Ui::Dialog *ui;
    QStringList* messageList;
    QStringList* userList;
    QString instrumentCode;//选中的仪器编码
    int flag;//区分是部门内移交窗口(1)和跨部门移交窗口(2)
    bool isRefresh; //判断信息是否成功录入sub界面按钮是否需要刷新
};

#endif // DIALOG_H
