#include "gzsystem.h"
#include "ui_gzsystem.h"
#include "departmentmanager.h"
#include "instrumentmanager.h"
#include "usermanager.h"

GZSystem::GZSystem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GZSystem)
{
    ui->setupUi(this);
}

GZSystem::~GZSystem()
{
    delete ui;
}

void GZSystem::on_pb_department_clicked()
{
    DepartmentManager w;
    w.exec();
}

void GZSystem::on_pb_user_clicked()
{
    UserManager w;
    w.exec();
}

void GZSystem::on_pb_instrument_clicked()
{
    InstrumentManager w;
    w.exec();
}
