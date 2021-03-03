#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QLabel>
#include<QString>
#include<QMessageBox>
#include<QColorDialog>
#include<QTimer>
#include<QDateTime>
#include<QFile>
#include<QTextStream>
#include<QThread>
#include<QDebug>
#include"buildmap.h"
#include"ui_buildmap.h"
#include"manager.h"
#include"usehelp.h"
#include"manager.h"
#include<string.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    Ui::MainWindow *ui;
    Buildmap* buildmap;                         //建立图
    Manager* manager;                           //后端
    Usehelp* usehelp;                           //使用帮助
    QPushButton *stackbutton[7],*queuebutton[7];//栈和队列展示
    int stackflag, queueflag;                   //栈和队列index
    QLabel* statuslabel1, *statuslabel2;        //状态栏
    QTimer *timer;                              //定时器

    void signal_slot();  //连接信号和槽
    void initstatusbar();//初始化状态栏
    bool check();        //查看是否可以遍历
private slots:
    //时间更新
    void timerupdate();
    //以邻接矩阵的方式确定一个图
    void on_actioncreate_triggered();
    //文本方式添加功能函数
    void addbytext();
    //更新邻接链表
    void adjlistupdate();
    //递归方式深度优先遍历
    void on_actiondfsnostack_triggered();
    //非递归方式深度优先遍历
    void on_actiondfsstack_triggered();
    //广度优先遍历
    void on_actionbfs_triggered();
    //使用帮助
    void on_actionhelp_triggered();
    //关于Qt
    void on_actionaboutqt_triggered();
    //关于作者
    void on_actionaboutauther_triggered();
};

#endif // MAINWINDOW_H
