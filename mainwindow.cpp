#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "proc.h"
#include "RefleshThread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initSysTab();
    initProcTab();
    initResTab();
    initAboutTab();


    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(onButtonEndProcClicked()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(onButtonShutDownClicked()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(onButtonReBootClicked()));

    thread = new RefleshThread();
    thread->start();
    connect(thread,SIGNAL(reflesh1(Proc )),this,SLOT(refleshProcTab(Proc )));
    connect(thread,SIGNAL(reflesh2(CpuRate)),this,SLOT(refleshSysTab(CpuRate)));
    connect(thread,SIGNAL(reflesh3(int,int)),this,SLOT(refleshMem(int,int)));
}

void MainWindow::initSysTab()
{
    QString vendor_id = getInfoByField("/proc/cpuinfo","vendor_id");
    QString model_name = getInfoByField("/proc/cpuinfo","model name");
    QString cpu_MHz = getInfoByField("/proc/cpuinfo","cpu MHz");
    QString cache_size = getInfoByField("/proc/cpuinfo","cache size");
    QString cpu_cores = getInfoByField("/proc/cpuinfo","cpu cores");


    QString cpuinfo = getContentFromCmd("cat /proc/cpuinfo");
    QString kernel_name = getContentFromCmd("uname -s");
    QString kernel_release = getContentFromCmd("uname -r");
    QString host_name = getContentFromCmd("uname -n");
    QString sys_name = getContentFromCmd("cat /etc/issue");
    QString mem_size = getInfoByField("/proc/meminfo","MemTotal");
    QString complier_version = getContentFromCmd("gcc --version | awk \'NR==1\'");

    if (vendor_id == "AuthenticAMD") {
        ui->label->setPixmap(QPixmap(":/new/image/amd.png"));
    }else if(vendor_id == "GenuineIntel"){
        ui->label->setPixmap(QPixmap(":/new/image/intel.png"));
    }

    ui->label_10->setText(host_name);
    ui->textEdit->setText(cpuinfo);
    ui->lineEdit->setText(sys_name);
    ui->lineEdit_2->setText(kernel_name+' '+kernel_release);
    ui->lineEdit_3->setText(complier_version);
    ui->lineEdit_4->setText(mem_size);
    ui->lineEdit_5->setText(model_name);
    ui->lineEdit_6->setText(cpu_MHz);
    ui->lineEdit_7->setText(cpu_cores);
    ui->lineEdit_8->setText(cache_size);

    ui->label_15->setText(getSysStartTime());

}

void MainWindow::refleshSysTab(CpuRate rate)
{
    QString str;
    ui->label_20->setText(str.sprintf("%.1f",rate.cpu[0]));
    ui->label_16->setText(getSysUptime());
    ui->label_17->setText(getContentFromCmd("uptime"));


    ui->label_21->setText(str.sprintf("Cpu0: %d",(int)rate.cpu[1]));
    ui->label_22->setText(str.sprintf("Cpu1: %d",(int)rate.cpu[2]));
    ui->label_23->setText(str.sprintf("Cpu2: %d",(int)rate.cpu[3]));
    ui->label_24->setText(str.sprintf("Cpu3: %d",(int)rate.cpu[4]));

    for (int i=0;i<4;i++)
    {
        m_pCurve[i]->updateData((int)rate.cpu[i+1]);
    }



}

void MainWindow::initProcTab()
{
    QStringList header;
    header<<"进程名"<<"用户"<<"% Cpu"<<"ID"<<"状态"<<"内存"<<"运行时间";

    ui->tableWidget->setColumnCount(header.count());
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setColumnWidth(0,120);
    ui->tableWidget->setColumnWidth(1,70);
    ui->tableWidget->setColumnWidth(2,50);
    ui->tableWidget->setColumnWidth(3,50);
    ui->tableWidget->setColumnWidth(4,70);
    ui->tableWidget->setColumnWidth(5,70);
    ui->tableWidget->setColumnWidth(6,100);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->verticalHeader()->setHidden(true);

}

void MainWindow::setTableViewCell(int row, ProcessInfo proinfo)
{
    ui->tableWidget->setItem(row,0,new QTableWidgetItem(proinfo.pname));
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(proinfo.puser));
    ui->tableWidget->setItem(row,2,new QTableWidgetItem(proinfo.pcpu));
    ui->tableWidget->setItem(row,3,new QTableWidgetItem(proinfo.pid));
    ui->tableWidget->setItem(row,4,new QTableWidgetItem(proinfo.pstatus));
    ui->tableWidget->setItem(row,5,new QTableWidgetItem(proinfo.pmem));
    ui->tableWidget->setItem(row,6,new QTableWidgetItem(proinfo.ptime));
}

void MainWindow::refleshProcTab(Proc proc)
{
    int rowCount = proc.procinfo.count();
    ui->tableWidget->setRowCount(rowCount);
    ui->label_18->setText(QString::number(rowCount));
    for (int i=0;i<rowCount;++i)
    {
        setTableViewCell(i,proc.procinfo.at(i));
    }
}


void MainWindow::onButtonEndProcClicked()
{
    if (!ui->tableWidget->currentItem()) return;
    int row = ui->tableWidget->currentItem()->row();
    if (row >= 0)
    {
        QString message = QString("结束pid为%1的进程").arg(ui->tableWidget->takeItem(row,3)->text());
        QMessageBox::StandardButton rb = QMessageBox::question(this,"End Process",message, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            system(QString("kill -9 %1").arg(ui->tableWidget->takeItem(row,3)->text()).toStdString().c_str());
        }
    }
}

void MainWindow::onButtonReBootClicked()
{
    QString message = QString("确定重启吗");
    QMessageBox::StandardButton rb = QMessageBox::question(this,"重启",message, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        system("reboot");
    }
}

void MainWindow::onButtonShutDownClicked()
{
    QString message = QString("确定关机吗");
    QMessageBox::StandardButton rb = QMessageBox::question(this,"关机",message, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        system("shutdown");
    }
}


void MainWindow::initResTab()
{
    for (int i=0;i<4;i++)
    {
        m_pCurve[i] = new Curve;
        m_pCurve[i]->init(5, 30, 20, -5, 100);
        if(i==0) ui->verticalLayout1->addWidget((Curve *)m_pCurve[i]);
        else if(i==1) ui->verticalLayout2->addWidget((Curve *)m_pCurve[i]);
        else if(i==2) ui->verticalLayout3->addWidget((Curve *)m_pCurve[i]);
        else if(i==3) ui->verticalLayout4->addWidget((Curve *)m_pCurve[i]);
    }


    QString buf = getInfoByField("/proc/meminfo","MemTotal");
    sscanf(buf.toStdString().c_str(),"%d",&memtotal);
    int mem = memtotal;
    buf = getInfoByField("/proc/meminfo","SwapTotal");
    sscanf(buf.toStdString().c_str(),"%d",&swaptotal);
    ui->label_27->setText(QString::number(mem).append("KB"));
    ui->label_35->setText(QString::number(swaptotal).append("KB"));


    m_pCurve[5] = new Curve;
    m_pCurve[5]->init(5, 110, 20, -5, mem);
    ui->verticalLayout5->addWidget((Curve *)m_pCurve[5]);

}

void MainWindow::refleshMem(int memfree, int swapfree)
{
    m_pCurve[5]->updateData(memfree);
    ui->label_28->setText(QString::number(memfree).append("KB"));
    ui->label_37->setText(QString::number(swapfree).append("KB"));

    QString str;
    ui->label_34->setText(str.sprintf("%.1f",memfree/(double)memtotal));
}

void MainWindow::initAboutTab()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
