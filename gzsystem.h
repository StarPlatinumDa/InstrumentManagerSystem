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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::GZSystem *ui;
};

#endif // GZSYSTEM_H
