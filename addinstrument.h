#ifndef ADDINSTRUMENT_H
#define ADDINSTRUMENT_H

#include <QDialog>
namespace Ui {
class AddInstrument;
}

class AddInstrument : public QDialog
{
    Q_OBJECT

public:
    explicit AddInstrument(QWidget *parent = nullptr);
    ~AddInstrument();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_le_name_returnPressed();

    void on_le_number_returnPressed();

    void on_pb_cancel_clicked();

    void on_pb_ok_clicked();

    void on_le_nodel_returnPressed();

    void on_dsp_price_editingFinished();

private:
    Ui::AddInstrument *ui;
    void writeFile(QString path,QString data);
};

#endif // ADDINSTRUMENT_H
