#ifndef LOGGING_H
#define LOGGING_H

#include <QDialog>

namespace Ui {
class Logging;
}

class Logging : public QDialog
{
    Q_OBJECT

public:
    explicit Logging(QWidget *parent = nullptr);
    ~Logging();

private slots:
    void on_pb_quit_clicked();

    void on_pb_ok_clicked();

    void on_le_username_returnPressed();

    void on_le_password_returnPressed();

private:
    Ui::Logging *ui;
    QString username;
    QString password;
    QString encode(QString &s);
    QStringList *users;
    bool readFile(QString path,QStringList *container);
};

#endif // LOGGING_H
