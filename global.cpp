#include "global.h"

Global::Global()
{
    userid="";
    usertype=-1;
}

Global* Global::global=new Global;
Global* Global::getInstant()
{
    return global;
}

QString Global::getUserid()
{
    return userid;
}

void Global::setUserid(const QString &value)
{
    userid = value;
}

int Global::getUsertype()
{
    return usertype;
}

void Global::setUsertype(const int &value)
{
    usertype = value;
}
