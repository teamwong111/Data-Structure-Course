#include "manager.h"

//构造函数
Manager::Manager(){}

//文件读取
bool Manager::readfile(QString filename)
{
    memset(adj,0,sizeof(adj));
    for(int i=0;i<n;i++)
    {
        link *t=&adjl[i];
        link *p;
        while(t!=NULL)
        {
            p=t;
            t=t->next;
            delete p;
        }
    }
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QTextStream in(&file);
    while(!in.atEnd())
    {
        in>>n;
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                in>>adj[i][j];
            }
        }
    }
    file.close();
    if(n==0)
        return false;
    return true;
}

//由邻接矩阵转为邻接链表
void Manager::toadjlist()
{
    for (int i = 0; i < n; i++)
    {
        link *t = &adjl[i];
        t->id = i;
        for (int j = 0; j < n; j++)
        {
            if (true == adj[i][j])
            {
                link* temp=new link;
                temp->id = j;
                temp->next = NULL;
                t->next = temp;
                t = t->next;
            }
        }
    }
}

//递归方式深度优先遍历
void Manager::dfs_nostack(int id, QString &plus)
{
    link* p = &adjl[id];
    while (p != NULL)
    {
        if (p != NULL && visit[p->id] == 0)
        {
            visit[p->id] = 1;
            plus+="->";
            plus+=QString::number(p->id);
            dfs_nostack(p->id, plus);
        }
        else
            p = p->next;
    }
}

//非递归方式深度优先遍历
void Manager::dfs_stack(int id, QString &plus)
{
    link* p = adjl[id].next;
    while(p != NULL)
    {
        s.push(p->id);
        show+=QString::number(p->id);
        show+='i';
        p = p->next;
    }
    while (!s.empty())
    {
        int temp = s.top();
        if (visit[temp] == 0)
        {
            plus+="->";
            plus+=QString::number(temp);
            visit[temp] = 1;
        }
        s.pop();
        show+=QString::number(temp);
        show+='o';
        link* t = adjl[temp].next;
        while (t != NULL)
        {
            if (visit[t->id] == 0)
            {
                s.push(t->id);
                show+=QString::number(t->id);
                show+='i';
                t = t->next;
            }
            else
                t = t->next;
        }
    }
}

//广度优先遍历
void Manager::bfs(int id, QString &plus)
{
    link* p = adjl[id].next;
    while (p != NULL)
    {
        q.enqueue(p->id);
        show+=QString::number(p->id);
        show+='i';
        p = p->next;
    }
    while (!q.empty())
    {
        int temp = q.front();
        q.dequeue();
        show+=QString::number(temp);
        show+='o';
        if (visit[temp] == 0)
        {
            plus+="->";
            plus+=QString::number(temp);
            visit[temp] = 1;
        }
        link* t = adjl[temp].next;
        while (t != NULL)
        {
            if (visit[t->id] == 0)
            {
                q.enqueue(t->id);
                show+=QString::number(t->id);
                show+='i';
                t = t->next;
            }
            else
                t = t->next;
        }
    }
}
