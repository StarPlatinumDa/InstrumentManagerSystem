#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
class Global
{
private:
    explicit Global();
private:
    int usertype;
    QString userid;
    QString instrumentName;
    static Global *global;
public:
    int getUsertype();
    void setUsertype(const int &value);
    QString getUserid();
    void setUserid(const QString &value);
    static Global* getInstant();
    void setInstrumentName(const QString &value);

};

#endif // GLOBAL_H
