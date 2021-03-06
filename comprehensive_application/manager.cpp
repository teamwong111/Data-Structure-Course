#include "manager.h"

//构造函数
Manager::Manager(){}

//从文件读取数据
bool Manager::readfile(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QTextStream in(&file);
    while(!in.atEnd())
    {
        bool ok;
        int total, lineindex, stationindex1, stationindex2;
        QString lineid, linename, linecolors, fromto, stationlist, linecolor, fromstation, tostation, longlat;
        QStringList strList;
        Line line;
        Station station;
        in>>lineid>>line.id;
        in>>linename>>line.name;
        in>>linecolors>>linecolor;
        line.color.setRgba(linecolor.remove(0,1).toUInt(&ok, 16));
        in>>fromto>>fromstation>>tostation;
        in>>stationlist>>total;
        line.fromto.push_back(fromstation);
        line.fromto.push_back(tostation);
        if (lineshash.count(line.name))
        {
            lineindex = lineshash[line.name];
            lines[lineindex].fromto.push_back(fromstation);
            lines[lineindex].fromto.push_back(tostation);
        }
        else
        {
            lineshash[line.name] = lines.size();
            lineindex = lines.size();
            lines.push_back(line);
        }
        for (int i=0; !in.atEnd()&&i<total; ++i)
        {
            in>>station.id>>station.name>>longlat;
            strList=longlat.split(QChar(','));
            station.longitude=strList.first().toDouble();
            station.latitude=strList.last().toDouble();
            if (stationshash.count(station.name))
            {
                stationindex2 = stationshash[station.name];
            }
            else
            {
                stationindex2 = stationshash[station.name] = stations.size();
                stations.push_back(station);
            }
            stations[stationindex2].theline.insert(lineindex);
            lines[lineindex].stationset.insert(stationindex2);
            if (i)
            {
                lines[lineindex].edges.insert(Edge(stationindex1, stationindex2));
                lines[lineindex].edges.insert(Edge(stationindex2, stationindex1));
                insertedge(stationindex1, stationindex2);
            }
            stationindex1 = stationindex2;
        }
        bool flag = lineid=="lineid:" && linename=="linename:" && linecolors=="linecolors:" && fromto=="fromto:" && stationlist=="stationlist:" && ok && !in.atEnd();
        if(flag==false)
        {
            file.close();
            cleardata();
            return false;
        }
        in.readLine();
    }
    file.close();
    boundlongilatiupdate();
    return true;
}

//清空数据
void Manager::cleardata()
{
    stations.clear();
    lines.clear();
    stationshash.clear();
    lineshash.clear();
    edges.clear();
    graph.clear();
}

//插入一条边
bool Manager::insertedge(int e1, int e2)
{
    if (edges.contains(Edge(e1, e2)) || edges.contains(Edge(e2, e1)))
    {
        return false;
    }
    edges.insert(Edge(e1, e2));
    return true;
}

//生成图结构
void Manager::tograph()
{
    graph.clear();
    graph=QVector<QVector<Node>>(stations.size(), QVector<Node>());
    for (auto &a : edges)
    {
        double dist=distance(stations[a.first].latitude, stations[a.first].longitude, stations[a.second].latitude, stations[a.second].longitude);
        graph[a.first].push_back(Node(a.second, dist));
        graph[a.second].push_back(Node(a.first, dist));
    }
}

//获取线路颜色
QColor Manager::getlinecolor(int i)
{
    return lines[i].color;
}

//获取线路名
QString Manager::getlinename(int i)
{
    return lines[i].name;
}

//获取线路名列表
QList<QString> Manager::getlinenamelist()
{
    QList<QString> linenamelist;
    for (auto a:lines)
    {
        linenamelist.push_back(a.name);
    }
    return linenamelist;
}

//获取线路hash值
int Manager::getlinehash(QString linename)
{
    if(lineshash.contains(linename))
    {
        return lineshash[linename];
    }
    return -1;
}

//获取线路hash值列表
QList<int> Manager::getlinehashlist(QList<QString> linelist)
{
    QList<int> hashlist;
    for (auto &a:linelist)
    {
        hashlist.push_back(getlinehash(a));
    }
    return hashlist;
}

//获取线路所有站点
QList<QString> Manager::getstationlist(int i)
{
    QList<QString> stationlist;
    for (auto &a:lines[i].stationset)
    {
        stationlist.push_back(stations[a].name);
    }
    return stationlist;
}

//更新边界经纬度
void Manager::boundlongilatiupdate()
{
    double minLongitude=200, minLatitude=200;
    double maxLongitude=0, maxLatitude=0;
    for (auto &s : stations)
    {
        minLongitude = qMin(minLongitude, s.longitude);
        minLatitude = qMin(minLatitude, s.latitude);
        maxLongitude = qMax(maxLongitude, s.longitude);
        maxLatitude = qMax(maxLatitude, s.latitude);
    }
    theminLongitude = minLongitude;
    theminLatitude = minLatitude;
    themaxLongitude = maxLongitude;
    themaxLatitude = maxLatitude;
}

//获取站点最小坐标
QPointF Manager::getmincoord()
{
    return QPointF(theminLongitude, theminLatitude);
}

//获取站点最大坐标
QPointF Manager::getmaxcoord()
{
    return QPointF(themaxLongitude, themaxLatitude);
}

//获取两个站点的公共所属线路
QList<int> Manager::getcommonlines(int s1, int s2)
{
    QList<int> linelist;
    for (auto &s : stations[s1].theline)
    {
        if(stations[s2].theline.contains(s))
            linelist.push_back(s);
    }
    return linelist;
}

//获取站点名
QString Manager::getstationname(int i)
{
    return stations[i].name;
}

//获取站点地理坐标
QPointF Manager::getstationcoord(int i)
{
    return QPointF(stations[i].longitude, stations[i].latitude);
}

//获取站点所属线路
QList<int> Manager::getstationlines(int i)
{
    return stations[i].theline.toList();
}

//获取站点hash值
int Manager::getstationhash(QString stationname)
{
    if(stationshash.contains(stationname))
    {
        return stationshash[stationname];
    }
    return -1;
}

//获取站点hash值列表
QList<QString> Manager::getstationnamelist()
{
    QList<QString> stationnamelist;
    for (auto &a: stations)
    {
        stationnamelist.push_back(a.name);
    }
    return stationnamelist;
}

//获取站点间实地直线距离
int Manager::distance(double fLati1, double fLong1, double fLati2, double fLong2)
{
    const double EARTH_RADIUS = 6378.137;
    const double PI = 3.1415926535898;
    double radLat1 = fLati1 * PI / 180.0;
    double radLat2 = fLati2 * PI / 180.0;
    double a = radLat1 - radLat2;
    double b = fLong1 * PI / 180.0 - fLong2 * PI / 180.0;
    double s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
    s = s * EARTH_RADIUS;
    s = (int)(s * 10000000) / 10000;
    return s;
}

//添加新线路
void Manager::addline(QString linename, QColor color)
{
    lineshash[linename]=lines.size();
    lines.push_back(Line(linename,color));
}

//添加新站点
void Manager::addstation(Station s)
{
    int hash=stations.size();
    stationshash[s.name]=hash;
    stations.push_back(s);
    for (auto &a: s.theline)
    {
        lines[a].stationset.insert(hash);
    }
    boundlongilatiupdate();
}

//添加站点连接关系
void Manager::addconnect(int s1, int s2, int l)
{
    insertedge(s1,s2);
    lines[l].edges.insert(Edge(s1,s2));
    lines[l].edges.insert(Edge(s2,s1));
}

//获取网络结构
void Manager::getgeneralroadmap(QList<int>&stationlist, QList<Edge>&edgelist)
{
    stationlist.clear();
    for (int i=0; i<stations.size(); ++i)
    {
        stationlist.push_back(i);
    }
    edgelist=edges.toList();
}

//获取最少时间的线路
bool Manager::getmintimetransfer(int s1, int s2, QList<int>&stationlist, QList<Edge>&edgelist)
{
    const int INF = 999999999;
    stationlist.clear();
    edgelist.clear();
    if(s1==s2)
    {
        stationlist.push_back(s2);
        stationlist.push_back(s1);
        return true;
    }
    tograph();
    std::vector<int> path(stations.size(), -1);
    std::vector<double> dist(stations.size(), INF);
    dist[s1]=0;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> q;
    q.push(Node(s1, 0));
    while(!q.empty())
    {
        Node top=q.top();
        q.pop();
        if(top.station==s2)
        {
            break ;
        }

        for (int i=0; i<graph[top.station].size(); ++i)
        {
            Node &adjNode=graph[top.station][i];
            if(top.distance+adjNode.distance<dist[adjNode.station])
            {
                path[adjNode.station]=top.station;
                dist[adjNode.station]=top.distance+adjNode.distance;
                q.push(Node(adjNode.station, dist[adjNode.station]));
            }
        }
    }
    if(path[s2]==-1)
    {
        return false;
    }
    int p=s2;
    while(path[p]!=-1)
    {
        stationlist.push_front(p);
        edgelist.push_front(Edge(path[p],p));
        p=path[p];
    }
    stationlist.push_front(s1);
    return true;
}

//获取最少换乘的线路
bool Manager::getmintransferline(int s1, int s2, QList<int>&stationlist, QList<Edge>&edgelist)
{
    stationlist.clear();
    edgelist.clear();
    if(s1==s2)
    {
        stationlist.push_back(s2);
        stationlist.push_back(s1);
        return true;
    }
    std::vector<bool> linesVisted(lines.size(),false);
    std::vector<int> path(stations.size(),-1);
    path[s1]=-2;
    std::queue<int> q;
    q.push(s1);
    while(!q.empty())
    {
        int top=q.front();
        q.pop();
        for (auto &l: stations[top].theline)
        {
            if(!linesVisted[l])
            {
                linesVisted[l]=true;
                for (auto &s: lines[l].stationset)
                {
                    if(path[s]==-1)
                    {
                        path[s]=top;
                        q.push(s);
                    }
                }
            }
        }
    }
    if(path[s2]==-1)
    {
        return false;
    }
    int p=s2;
    while(path[p]!=-2)
    {
        stationlist.push_front(p);
        edgelist.push_front(Edge(path[p],p));
        p=path[p];
    }
    stationlist.push_front(s1);
    return true;
}
