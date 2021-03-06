#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mouseaction.h"
#include "manager.h"
#include "dynamicadd.h"
#include "usehelp.h"

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QColorDialog>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

#include<QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //视图放大
    void on_actionenlarge_triggered();
    //视图缩小
    void on_actionnarrow_triggered();
    //动态添加
    void on_actionadd_triggered();
    //地铁线路总图
    void on_actionmetronet_triggered();
    //使用帮助
    void on_actionhelp_triggered();
    //关于Qt
    void on_actionaboutqt_triggered();
    //关于作者
    void on_actionaboutauthor_triggered();
    //时间更新
    void timerupdate();
    //动态添加更新
    void tabWidgetupdate(int);
    //添加线路
    void addline();
    //添加站点
    void addstation();
    //添加连接
    void addconnect();
    //添加文本
    void addtext();
    //换乘路线更新
    void transfer_line_update();
    //换乘起始站点更新
    void transfer_startstation_update(const QString&);
    //换乘终点站更新
    void transfer_endstation_update(const QString&);
    //换乘查询槽函数
    void transferinquiry();

protected:
    Ui::MainWindow *ui;
    Mouseaction *mouseaction;           //鼠标缩放
    QGraphicsScene *scene;              //场景
    Manager* manager;                   //后端
    Dynamicadd* dynamicadd;             //动态添加
    Usehelp* usehelp;                   //使用帮助
    QTimer* timer;                      //计时器
    QLabel* statuslabel1, *statuslabel2;//状态栏

    //加载数据
    void loaddata();
    //连接信号和槽
    void signal_slot();
    //初始化状态栏
    void initstatusbar();
    //计算混合颜色
    QColor getmixcolor(const QList<int>&);
    //获得线路表的名字集
    QString getlinenames(const QList<int>&);
    //经纬度坐标转为视图坐标
    QPointF toviewcoord(QPointF);
    //绘制网络图的边
    void drawline(const QList<Edge>&);
    //绘制网络图的站点节点
    void drawstation(const QList<int>&);
};

#endif // MAINWINDOW_H
