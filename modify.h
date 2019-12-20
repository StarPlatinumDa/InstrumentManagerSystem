#ifndef MODIFY_H
#define MODIFY_H

#include <QDialog>

namespace Ui {
class Modify;
}

class Modify : public QDialog
{
    Q_OBJECT

public:
    explicit Modify(QWidget *parent = nullptr);
    ~Modify();
    QStringList *str;
    void re();//刷新
    void rebyclick(int i);//通过传入的行改变
    int flag;//为了实现确认的不同功能（0为新增，1为修改）
    int currentrow=0;//现在的行
    QString searchfuzeren(QString  f);//寻找负责人
    QString searchdepart(QString  f);//寻找负责人
    QStringList *fuzeren;//读取负责人文件
private slots:
    void on_pb_ok_clicked();
    void on_pb_cancel_clicked();

signals:
    void addcomplete();
private:
    Ui::Modify *ui;
};

#endif // MODIFY_H
