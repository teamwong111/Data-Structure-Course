#ifndef MANAGER_H
#define MANAGER_H

#include<QQueue>
#include<QStack>
#include<QString>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<string.h>
#include<QMessageBox>
struct link
{
   int id;
   link* next;
};
class Manager
{
public:
    //构造函数
    Manager();
    //文件读取
    bool readfile(QString filename);
    //由邻接矩阵转为邻接链表
    void toadjlist();
    //递归方式深度优先遍历
    void dfs_nostack(int id, QString &plus);
    //非递归方式深度优先遍历
    void dfs_stack(int id, QString &plus);
    //广度优先遍历
    void bfs(int id, QString &plus);

    int n=0;            //节点数
    int adj[100][100];  //邻接矩阵
    link adjl[100];     //邻接链表
    bool visit[100];    //是否访问过
    QStack<int> s;      //栈
    QQueue<int> q;      //队列
    QString show;       //展示用

    friend class MainWindow;//友元
};

#endif // MANAGER_H
