#include "gzsystem.h"
#include <QApplication>
#include "logging.h"
#include "global.h"
#include "individual.h"
#include "depart.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Logging w;
    Global *temp=Global::getInstant();
    if(w.exec()==QDialog::Accepted)
    {
        if(temp->getUsertype()==0){
            GZSystem G;
            G.setWindowTitle("仪器设备管理系统(超级用户)");
            G.show();
            return a.exec();
        }else if(temp->getUsertype()==1){
            GZSystem G;
            G.show();
            return a.exec();
        }else if(temp->getUsertype()==2){
            depart G;
            G.show();
            return a.exec();
        }else{
            Individual G;
            G.show();
            return a.exec();
        }
    }
    return 0;
}
