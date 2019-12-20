#include "userinformation.h"
#include "ui_userinformation.h"

UserInformation::UserInformation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInformation)
{
    ui->setupUi(this);
}

UserInformation::~UserInformation()
{
    delete ui;
}

void UserInformation::setType(int value)
{
    type = value;
    if(type==1){
        this->ui->pb_ok->setText("保存");
    }
}
