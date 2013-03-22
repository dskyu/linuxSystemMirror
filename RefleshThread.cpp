#include "RefleshThread.h"

RefleshThread::RefleshThread(QObject *parent) :
    QThread(parent)
{

}

void RefleshThread::run()
{
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(refleshSysTab()));
    timer->start(1000);

    QTimer *timer2 = new QTimer(this);
    connect(timer2,SIGNAL(timeout()),this,SLOT(refleshProcTab()));
    timer2->start(1000);

    exec();
}

void RefleshThread::refleshProcTab()
{
    emit reflesh1(Proc());
}

void RefleshThread::refleshSysTab()
{
    CpuStat cpuinfo[5][2];
    CpuRate rate;

    for (int i=0;i<5;i++)
    {
        cpuinfo[i][0] = CpuStat(getCpuInfo(i+1));
    }
    usleep(100000);
    for (int i=0;i<5;i++)
    {
        cpuinfo[i][1] = CpuStat(getCpuInfo(i+1));
        rate.cpu[i] = 100-100*(cpuinfo[i][1].idle-cpuinfo[i][0].idle)/(double)(cpuinfo[i][1].sum - cpuinfo[i][0].sum);
    }

    std::cout << rate.cpu[0] << "  " << rate.cpu[1] << "  " << rate.cpu[2] << "  " << rate.cpu[3] << "  " << rate.cpu[4] << endl;

    emit reflesh2(rate);


    int memfree;
    int swapfree;
    QString buf = getInfoByField("/proc/meminfo","MemFree");
    sscanf(buf.toStdString().c_str(),"%d",&memfree);
    buf = getInfoByField("/proc/meminfo","SwapFree");
    sscanf(buf.toStdString().c_str(),"%d",&swapfree);

    emit reflesh3(memfree,swapfree);
}

void RefleshThread::refleshCpuRate()
{

}
