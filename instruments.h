#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include <QDialog>

namespace Ui {
class instruments;
}

class instruments : public QDialog
{
    Q_OBJECT

public:
    explicit instruments(QWidget *parent = nullptr);
    ~instruments();

private:
    Ui::instruments *ui;
};

#endif // INSTRUMENTS_H
