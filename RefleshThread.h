#ifndef REFLESHTHREAD_H
#define REFLESHTHREAD_H

#include <QThread>
#include <QTimer>
#include <iostream>
#include "proc.h"
using namespace std;

class CpuRate
{
public:
    double cpu[5];
};


class RefleshThread : public QThread
{
    Q_OBJECT
public:
    explicit RefleshThread(QObject *parent = 0);

signals:
    void reflesh1(Proc);
    void reflesh2(CpuRate);
    void reflesh3(int ,int);
public slots:
    void refleshSysTab();
    void refleshProcTab();
    void refleshCpuRate();
protected:
    void run();
};

#endif // REFLESHTHREAD_H
