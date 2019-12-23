#ifndef GZSYSTEM_H
#define GZSYSTEM_H

#include <QMainWindow>

namespace Ui {
class GZSystem;
}

class GZSystem : public QMainWindow
{
    Q_OBJECT

public:
    explicit GZSystem(QWidget *parent = nullptr);
    ~GZSystem();

private slots:

    void on_pb_department_clicked();

    void on_pb_user_clicked();

    void on_pb_instrument_clicked();

private:
    Ui::GZSystem *ui;
};

#endif // GZSYSTEM_H
