#ifndef DEPARTMNET_H
#define DEPARTMNET_H
#include<iostream>
#include<QString>
#include<QDebug>

using namespace std;
class Depart
{
public:
    Depart(QString co,QString na,QString pco,QString des)//构造函数
    {
        code=co;name=na;pcode=pco;description=des;
    }
    Depart(){}
    ~Depart(){}
    QString getCode() const;
    void setCode(const QString &value);
    QString getName() const;
    void setName(const QString &value);
    QString getDescription() const;
    void setDescription(const QString &value);
    QString getPcode() const;
    void setPcode(const QString &value);
    static void DeleteOneline(int nNum, QString &strall);//删除一行
    static void deleteOnelineInFile(int nNumLine, QString filename);//删除一行
    //Depart p[20];//成员变量

private:
    QString code;//部门编号
    QString name;//部门名称
    QString pcode;//负责人编号
    QString description;//部门描述

};

#endif // DEPARTMNET_H
