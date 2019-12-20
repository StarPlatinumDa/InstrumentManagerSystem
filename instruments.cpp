#include "instruments.h"
#include "ui_instruments.h"

instruments::instruments(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::instruments)
{
    ui->setupUi(this);
}

instruments::~instruments()
{
    delete ui;
}
