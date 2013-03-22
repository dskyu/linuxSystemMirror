#include "tool.h"
#include <stdio.h>
#include <string.h>
#include <QString>
#include <QDir>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <iostream>

char *mytrim(char s[])
{
   int i=strlen(s)-1;
   for(;i>=0;i--)
      if(s[i]!=' ' && s[i]!='\t'&& s[i]!='\r'&& s[i]!='\n')
         break;
   s[i+1]='\0';

   while(*s==' ' || *s=='\t') s++;
   return s;
}

const QString getInfoByField(const char *path,const char *field){
    FILE *stream = fopen(path, "rb");
    char *arg = 0;
    char *res = 0;
    char *p = 0;
    size_t size = 0;
    while (getdelim(&arg, &size, '\n', stream) != -1) {
        res = strstr(arg,field);
        if (res != 0) {
            p = strtok(res,":");
            p = strtok(NULL,",");
            p = mytrim(p);
            break;
        }
    }
    QString s(p);
    free(arg);
    fclose(stream);
    return s;
}

const QString getContentFromCmd(const QString &s)
{
    QString r;
    FILE *stream;
    char buf[1024];
    int len;
    if((stream = popen(s.toStdString().c_str(),"r"))==NULL)
    return "";
    while((len=fread(buf, sizeof(char), sizeof(buf)-1, stream))!=0)
    {
        r.append(mytrim(buf));
        memset(buf, 0, sizeof(buf));
    }

    pclose( stream );
    return r;
}

const QString getContentFromFile(const QString &s)
{
    QString r;
    FILE *stream;
    char buf[1024];
    int len;
    if((stream = fopen(s.toStdString().c_str(),"r"))==NULL)
    return "";
    while((len=fread( buf, sizeof(char), sizeof(buf)-1, stream))!=0)
    {
        r.append(buf);
        memset( buf, 0, sizeof(buf));
    }
    fclose( stream );
    return r;
}

const QString getCpuInfo(int n)
{
    FILE *stream;
    if((stream = fopen("/proc/stat","r"))==NULL) return "";

    char *arg = 0;
    size_t size = 0;
    QString res;
    for (int i=0;i<n;++i) getdelim(&arg, &size, '\n', stream);
    res = QString(arg);
    free(arg);
    fclose(stream);
    return res;
}

const QString getSysUptime()
{
    int uptime;
    FILE *stream;
    if((stream = fopen("/proc/uptime", "r"))==NULL) return "";

    fscanf(stream,"%d",&uptime);
    return QString::number(uptime).append("秒");
}

const QString getSysStartTime()
{
    int uptime;
    FILE *stream;
    if((stream = fopen("/proc/uptime", "r"))==NULL) return "";

    fscanf(stream,"%d",&uptime);
    QDateTime time = QDateTime::currentDateTime().addSecs(-uptime);
    return time.toString("yyyy-MM-dd hh:mm:ss ddd");
}


bool isDigitString(const QString& src)
{
    const char *s = src.toUtf8().data();
    while(*s && *s>='0' && *s<='9')s++;
    return !bool(*s);
}


