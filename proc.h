#ifndef PROC_H
#define PROC_H

#include <QtGui/QtGui>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include "tool.h"


class ProcessStat
{
public:
    int pid;
    char comm[256];
    char state;
    int ppid;
    int pgrp;
    int session;
    int tty_nr;
    int tpgid;
    unsigned flags;
    unsigned long minflt;
    unsigned long cminflt;
    unsigned long majflt;
    unsigned long cmajflt;
    unsigned long utime;
    unsigned long stime;
    unsigned long cutime;
    unsigned long cstime;
    unsigned long priority;
    long nice;
    long num_threads;
    long itrealvalue;
    unsigned long long starttime;
    unsigned long vsize;
    long rss;
    unsigned long rsslim;
    unsigned long startcode;
    unsigned long endcode;
    unsigned long startstack;
    unsigned long kstkesp;
    unsigned long kstkeip;
    unsigned long signal;
    unsigned long blocked;
    unsigned long sigignore;
    unsigned long sigcatch;
    unsigned long wchan;
    unsigned long nswap;
    unsigned long cnswap;
    int exit_signal;
    int processor;
    unsigned rt_priority;
    unsigned policy;
    unsigned long long delayacct_blkio_ticks;
    unsigned long guest_time;
    long cguest_time;
    unsigned long long proctime;
    QString origin;
    ProcessStat(const char *s=NULL)
    {
        set(s);
    }
    ProcessStat(const QString &s)
    {
        set(s.toStdString().c_str());
    }

    void set(const char *s)
    {
        if(s!=NULL)
        {
            sscanf(s,"%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %u %u %llu %lu %ld",
                   &pid,
                   comm,
                   &state,
                   &ppid,
                   &pgrp,
                   &session,
                   &tty_nr,
                   &tpgid,
                   &flags,
                   &minflt,
                   &cminflt,
                   &majflt,
                   &cmajflt,
                   &utime,
                   &stime,
                   &cutime,
                   &cstime,
                   &priority,
                   &nice,
                   &num_threads,
                   &itrealvalue,
                   &starttime,
                   &vsize,
                   &rss,
                   &rsslim,
                   &startcode,
                   &endcode,
                   &startstack,
                   &kstkesp,
                   &kstkeip,
                   &signal,
                   &blocked,
                   &sigignore,
                   &sigcatch,
                   &wchan,
                   &nswap,
                   &cnswap,
                   &exit_signal,
                   &processor,
                   &rt_priority,
                   &policy,
                   &delayacct_blkio_ticks,
                   &guest_time,
                   &cguest_time
                   );
            origin = s;
            proctime = utime+stime+cutime+cstime;

           // std::cout << proctime << std::endl;
        }
    }

    ProcessStat &operator =(const char *s)
    {
        set(s);
        return *this;
    }
};

class ProcessStatm
{
public:
    unsigned long long size;
    unsigned long long resident;
    unsigned long long share;
    unsigned long long text;
    unsigned long long lib;
    unsigned long long data;
    unsigned long long dt;
    QString origin;
    void set(const char *s)
    {
        if(s!=NULL)
        {
            sscanf(s,"%llu %llu %llu %llu %llu %llu %llu",
                   &size,
                   &resident,
                   &share,
                   &text,
                   &lib,
                   &data,
                   &dt);
            origin = s;
        }
    }
    ProcessStatm(const char *s=NULL)
    {
        set(s);
    }
    ProcessStatm(const QString &s)
    {
        set(s.toStdString().c_str());
    }

    ProcessStatm &operator =(const char *s)
    {
        set(s);
        return *this;
    }
};

class CpuStat
{
public:
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long stealstolen;
    unsigned long long guest;
    unsigned long long sum;
    QString origin;
    void set(const char *s)
    {
        char buf[100];
        if(s!=NULL)
        {
            sscanf(s,"%s %llu %llu %llu %llu %llu %llu %llu %llu %llu",
                   buf,
                   &user,
                   &nice,
                   &system,
                   &idle,
                   &iowait,
                   &irq,
                   &softirq,
                   &stealstolen,
                   &guest);
            origin = s;
            sum = user+nice+system+idle+iowait+irq+softirq+stealstolen+guest;

        //    std::cout << s << "     " << sum << std::endl;
        }
    }
    CpuStat(const char *s=NULL)
    {
        set(s);
    }
    CpuStat(const QString &s)
    {
        set(s.toStdString().c_str());
    }

    CpuStat &operator =(const char *s)
    {
        set(s);
        return *this;
    }
};


class ProcessInfo
{
public:
    QString base;
    double cputotal;
    double cpuidle;
    double proctotal;
    QString pname;
    QString puser;
    QString pcpu;
    QString pid;
    QString pstatus;
    QString pmem;
    QString ptime;

    ProcessStat stat;
    ProcessStatm statm;
    QString status;
    ProcessInfo(QString pid):
        base(QString("/proc/").append(pid)),
        pid(pid),
        stat(getContentFromFile(QString(base).append("/stat"))),
        statm(getContentFromFile(QString(base).append("/statm"))),
        status(QString(base).append("/status"))
    {
        pname = getInfoByField(status.toStdString().c_str(),"Name");

        QString str = getInfoByField(status.toStdString().c_str(),"Uid");
        uid_t uid;
        sscanf(str.toStdString().c_str(),"%d",&uid);
        struct passwd *passwd;
        passwd = getpwuid (uid);

        puser = passwd->pw_name;

        CpuStat cpuinfo(getCpuInfo(1));

        cputotal = cpuinfo.sum;
        proctotal = stat.proctime;

        char buf[5];
        sprintf(buf,"%.1f",100*proctotal/cputotal);
        pcpu = QString(buf);

        this->pid = pid;

        switch (stat.state)
        {
        case 'R':
            pstatus = ("运行中");
            break;
        case 'S':
            pstatus = ("睡眠中");
            break;
        case 'D':
            pstatus = ("不可中断");
            break;
        case 'Z':
            pstatus = ("僵死");
            break;
        case 'T':
            pstatus = ("停止");
            break;
        case 'W':
            pstatus = ("无驻留页");
            break;
        default:
            pstatus = ("其它");
            break;
        }

        pmem = getInfoByField(status.toStdString().c_str(),"VmRSS");
        if (pmem == "") pmem = "0";

        ptime = QString::number(stat.proctime/sysconf(_SC_CLK_TCK));

 //       std::cout << pid.toStdString() << " " << pcpu.toStdString() << std::endl;
    }
    ProcessInfo()
    {
    }
};

class Proc
{
public:
    QVector<ProcessInfo> procinfo;
    Proc()
    {
        QString path("/proc/");
        QDir dir(path);

        QFileInfoList list = dir.entryInfoList(QDir::Dirs);
        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            if (isDigitString(fileInfo.fileName())){
                ProcessInfo procInfo(fileInfo.fileName());
                procinfo.push_back(procInfo);
            }
        }
    }

};

#endif // PROC_H
