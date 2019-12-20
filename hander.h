#ifndef HANDER_H
#define HANDER_H

#include <QObject>

class Hander : public QObject
{
    Q_OBJECT
private:
    explicit Hander(QObject *parent = nullptr);
signals:
    void addInstrument(QString name);
public slots:
private:
    static Hander *hand;
public:
    void sendName(QString name);
    static Hander* getInstant();
};

#endif // HANDER_H
