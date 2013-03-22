#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QtGui>
#include "RefleshThread.h"
#include "proc.h"
#include "curve.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    RefleshThread *thread;
    Proc *proc;
    Ui::MainWindow *ui;
    Curve* m_pCurve[5];

    int memtotal;
    int swaptotal;

    void initSysTab();
    void initProcTab();
    void initResTab();
    void initAboutTab();
    void setTableViewCell(int row,ProcessInfo proinfo);

public slots:
    void onButtonEndProcClicked();
    void onButtonReBootClicked();
    void onButtonShutDownClicked();
    void refleshSysTab(CpuRate rate);
    void refleshProcTab(Proc );
    void refleshMem(int memfree,int swapfree);
};

#endif // MAINWINDOW_H
