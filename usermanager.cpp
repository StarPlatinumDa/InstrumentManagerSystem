#include "usermanager.h"
#include "ui_usermanager.h"
#include "userinformation.h"
UserManager::UserManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserManager)
{
    ui->setupUi(this);
}

UserManager::~UserManager()
{
    delete ui;
}

void UserManager::on_pushButton_clicked()
{
    UserInformation w;
    w.exec();
}
