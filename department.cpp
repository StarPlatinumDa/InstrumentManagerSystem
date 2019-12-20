#include<department.h>
#include<QFile>
#include<QTextStream>
#include<QTextCodec>
QString Depart::getDescription() const
{
    return description;
}

void Depart::setDescription(const QString &value)
{
    description = value;
}

QString Depart::getPcode() const
{
return pcode;
}

void Depart::setPcode(const QString &value)
{
pcode = value;
}


QString Depart::getCode() const
{
return code;
}

void Depart::setCode(const QString &value)
{
code = value;
}

QString Depart::getName() const
{
    return name;
}

void Depart::setName(const QString &value)
{
    name = value;
}
void Depart::DeleteOneline(int nNum, QString &strall)
{
    int nLine=0;
    int Index=0;
    //算出行数nLine
    while(Index!=-1)
    {
        Index=strall.indexOf('\n',Index+1);
        nLine++;
    }

    //如果是直接从位置0开始删除\n算一个字符"abc\nme\ndo" \n的index是3要删除3+1个字符，即index+1个
    if(nNum==0)
    {
        int nIndex=strall.indexOf('\n');
        strall.remove(0,nIndex+1);
    }
    else
    {
        int nTemp=nNum;
        int nIndex=0,nIndex2=0;
        while(nTemp--)
        {
            //
            nIndex=strall.indexOf('\n',nIndex+1);//这里会更新nIndex
            if(nIndex!=-1)//说明是有效的
            {
                nIndex2=strall.indexOf('\n',nIndex+1);
            }
        }
        //删除的行不是最后一行（从nIndex+1这个位置起nIndex2-nIndex个字符全部抹去）
        if(nNum<nLine-1)
        {
            strall.remove(nIndex+1, nIndex2-nIndex);//不用减一
        }
        //删除的是最后一行（从nIndex起始len-nIndex个字符全抹去）
        //不能从nIndex+1处开始，
        else if(nNum==nLine-1)
        {
            int len=strall.length();
            strall.remove(nIndex,len-nIndex);
        }
        else
        {

        }

    }
}
void Depart::deleteOnelineInFile(int nNumLine, QString filename)
{
    QString strall;
    QFile readfile(filename);

    if(readfile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&readfile);
        stream.setCodec(QTextCodec::codecForName("utf-8"));
        strall=stream.readAll();
    }
    readfile.close();
    DeleteOneline(nNumLine, strall);

    QFile writefile(filename);
    if(writefile.open(QIODevice::WriteOnly))
    {
        QTextStream wrtstream(&writefile);
        wrtstream.setCodec(QTextCodec::codecForName("utf-8"));
        wrtstream<<strall;
    }
    writefile.close();
}

