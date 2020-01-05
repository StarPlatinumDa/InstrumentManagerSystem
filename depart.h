#ifndef DEPART_H
#define DEPART_H

#include <QMainWindow>
#include"sub1.h"
QT_BEGIN_NAMESPACE
namespace Ui { class depart; }
QT_END_NAMESPACE

class depart : public QMainWindow
{
    Q_OBJECT

public:
    depart(QWidget *parent = nullptr);
    ~depart();
    void readfile(QString filename,QStringList *str);//读文件
    QStringList *backapply;//设备归还申请
    QStringList *useapply;//设备使用申请
    QStringList *allapply;//读取文件中所有信息
    QStringList *thisapply;//分离出本部门的所有信息
    QStringList *dep;//读取所有部门信息
    QString finddepartbyid(QString id);//通过用户id找到他的部门
protected:

private slots:
    void on_pb_enter_clicked();

private:
    Ui::depart *ui;
};
#endif // DEPART_H
