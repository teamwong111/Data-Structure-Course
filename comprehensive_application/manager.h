#ifndef MANAGER_H
#define MANAGER_H

#include <QString>
#include <QPoint>
#include <QVector>
#include <QHash>
#include <QColor>
#include <QPair>
#include <QSet>
#include <QFile>
#include <QTextStream>
#include<queue>
#include<QDebug>

typedef QPair<int,int> Edge;//定义边类型

//线路类
struct Line
{
    int id;                  //线路ID
    QString name;            //线路名称
    QColor color;            //线路颜色
    QVector <QString> fromto;//线路起始站点
    QSet<int> stationset;    //线路站点集合
    QSet<Edge> edges;        //线路站点连接关系

    //构造函数
    Line(){}
    Line(QString lineName, QColor lineColor):name(lineName), color(lineColor){}
};

//地铁站点类定义
struct Station
{
    int id;                    //站点ID
    QString name;              //站点名称
    double longitude, latitude;//站点经纬度
    QSet<int> theline;         //站点所属线路

    //构造函数
    Station(){}
    Station(QString nameStr, double longi, double lati, QList<int> linesList):
        name(nameStr), longitude(longi), latitude(lati)
    {
        theline=linesList.toSet();
    }
};

//图的邻接点结构
struct Node
{
    int station;    //邻接点ID
    double distance;//两点距离

    //构造函数
    Node(){}
    Node(int s, double dist) :station(s), distance(dist){}

    //">"运算重载，用于小顶堆
    bool operator > (const Node& n) const
    {
        return this->distance>n.distance;
    }
};

//后端
class Manager
{
protected:
    QVector<Station> stations;       //存储所有站点
    QVector<Line> lines;             //存储所有线路
    QHash<QString, int> stationshash;//站点名到存储位置的hash
    QHash<QString, int> lineshash;   //线路名到存储位置的hash
    QSet<Edge> edges;                //所有边的集合
    QVector<QVector<Node>> graph;    //地铁线路网络图
    double theminLongitude, theminLatitude, themaxLongitude, themaxLatitude;//最小最大经纬度

public:
    //构造函数
    Manager();
    //获取线路名
    QString getlinename(int);
    //获取线路名列表
    QList<QString> getlinenamelist();
    //获取线路颜色
    QColor getlinecolor(int);
    //获取线路hash值
    int getlinehash(QString);
    //获取线路hash值列表
    QList<int> getlinehashlist(QList<QString>);
    //获取线路所有站点列表
    QList<QString> getstationlist(int);
    //获取站点名
    QString getstationname(int);
    //获取站点地理坐标
    QPointF getstationcoord(int);
    //获取站点hash值
    int getstationhash(QString);
    //获取站点命列表
    QList<QString> getstationnamelist();
    //获取站点最小坐标
    QPointF getmincoord();
    //获取站点最大坐标
    QPointF getmaxcoord();
    //获取站点所属线路
    QList<int> getstationlines(int);
    //获取两个站点的公共所属线路
    QList<int> getcommonlines(int, int);
    //获取站点间实地直线距离
    int distance(double, double, double, double);
    //添加新线路
    void addline(QString, QColor);
    //添加新站点
    void addstation(Station);
    //添加站点连接关系
    void addconnect(int, int, int);
    //获取网络结构
    void getgeneralroadmap(QList<int>&, QList<Edge>&);
    //获取最少时间的线路
    bool getmintimetransfer(int, int, QList<int>&, QList<Edge>&);
    //获取最少换乘的线路
    bool getmintransferline(int, int, QList<int>&, QList<Edge>&);
    //从文件读取数据
    bool readfile(QString);

private:
    //清空数据
    void cleardata();
    //插入一条边
    bool insertedge(int, int);
    //更新边界经纬度
    void boundlongilatiupdate();
    //生成图结构
    void tograph();
};

#endif // SUBWAYGRAPH_H
