#ifndef USERINFORMATION_H
#define USERINFORMATION_H

#include <QDialog>

namespace Ui {
class UserInformation;
}

class UserInformation : public QDialog
{
    Q_OBJECT

public:
    explicit UserInformation(QWidget *parent = nullptr);
    ~UserInformation();


    void setType(int value);

private:
    Ui::UserInformation *ui;
    int type;
};

#endif // USERINFORMATION_H
