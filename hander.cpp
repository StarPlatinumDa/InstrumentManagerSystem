#include "hander.h"

Hander::Hander(QObject *parent) : QObject(parent)
{

}

void Hander::sendName(QString name)
{
    emit addInstrument(name);
}

Hander *Hander::getInstant()
{
    return hand;
}
Hander* Hander::hand=new Hander;
