#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stackflag=0;
    stackbutton[0]=ui->pushButton;
    stackbutton[1]=ui->pushButton_2;
    stackbutton[2]=ui->pushButton_3;
    stackbutton[3]=ui->pushButton_4;
    stackbutton[4]=ui->pushButton_5;
    stackbutton[5]=ui->pushButton_6;
    stackbutton[6]=ui->pushButton_7;
    queueflag=0;
    queuebutton[0]=ui->pushButton_15;
    queuebutton[1]=ui->pushButton_14;
    queuebutton[2]=ui->pushButton_13;
    queuebutton[3]=ui->pushButton_12;
    queuebutton[4]=ui->pushButton_11;
    queuebutton[5]=ui->pushButton_10;
    queuebutton[6]=ui->pushButton_9;
    for(int i=0;i<7;++i)
    {
       stackbutton[i]->setVisible(false);
       stackbutton[i]->setStyleSheet("background-color: rgb(205, 133, 63)");
       queuebutton[i]->setVisible(false);
       queuebutton[i]->setStyleSheet("background-color: rgb(205,133,63)");
    }
    buildmap = new Buildmap(this);
    manager = new Manager;
    usehelp = new Usehelp;
    timer = new QTimer(this);
    timer->start(1000);
    signal_slot();
    initstatusbar();
    setWindowTitle("算法实现");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete buildmap;
    delete manager;
    delete usehelp;
    delete timer;
}

//连接信号和槽
void MainWindow::signal_slot()
{
    connect(buildmap->ui->pushButton, &QPushButton::clicked, this, &MainWindow::addbytext);
    connect(timer, &QTimer::timeout, this, &MainWindow::timerupdate);
}

//初始化状态栏
void MainWindow::initstatusbar()
{
    statuslabel1 = new QLabel;
    statuslabel1->setMinimumSize(200,15);
    statuslabel1->setFrameShape(QFrame::StyledPanel);
    statuslabel1->setFrameShadow(QFrame::Sunken);
    statuslabel1->setText(tr("欢迎来到算法实现"));
    statuslabel2 = new QLabel;
    statuslabel2->setMinimumSize(200,15);
    statuslabel2->setFrameShape(QFrame::StyledPanel);
    statuslabel2->setFrameShadow(QFrame::Sunken);
    statuslabel2->setText(tr("0000-00-00 00:00::00 星期 "));
    ui->statusBar->addWidget(statuslabel1);
    ui->statusBar->addWidget(statuslabel2);
}

//查看是否可以遍历
bool MainWindow::check()
{
    if(manager->n==0)
    {
        QMessageBox box;
        box.setWindowTitle("错误");
        box.setIcon(QMessageBox::Warning);
        box.setText("还未建立图");
        box.addButton("确定",QMessageBox::AcceptRole);
        if(box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
        return false;
    }
    return true;
}

//时间更新
void MainWindow::timerupdate()
{
    QDateTime time = QDateTime::currentDateTime();
    statuslabel2->setText(time.toString("yyyy-MM-dd hh:mm:ss dddd"));
}

//以邻接矩阵的方式确定一个图
void MainWindow::on_actioncreate_triggered()
{
    statuslabel1->setText(tr("以邻接矩阵的方式确定一个图"));
    ui->textBrowser->clear();
    ui->textBrowser_2->clear();
    ui->textBrowser_3->clear();
    buildmap->ui->textEdit->clear();
    buildmap->show();
}

//建立图
void MainWindow::addbytext()
{
    QMessageBox box;
    box.setWindowTitle(tr("建图"));
    QString filename="addtext.txt";
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);
    out<<buildmap->ui->textEdit->toPlainText();
    file.close();
    bool flag = manager->readfile(filename);
    if(flag)
    {
        box.setIcon(QMessageBox::Information);
        box.setText(tr("添加成功"));
        manager->toadjlist();
        adjlistupdate();
    }
    else
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("添加失败"));
    }
    box.addButton(tr("确定"),QMessageBox::AcceptRole);
    if(box.exec()==QMessageBox::Accepted)
    {
        box.close();
    }
}

//更新邻接链表
void MainWindow::adjlistupdate()
{
    statuslabel1->setText(tr("邻接链表已更新"));
    QString text="邻接链表如下:\n\n";
    for(int i=0; i<manager->n; i++)
    {
        link* temp = &manager->adjl[i];
        while(temp!=NULL)
        {
            text+=QString::number(temp->id);
            if(temp->next!=NULL)
                text+="->";
            temp=temp->next;
        }
        text+="\n\n";
    }
    ui->textBrowser->clear();
    ui->textBrowser->setText(text);
}

//递归方式深度优先遍历
void MainWindow::on_actiondfsnostack_triggered()
{
    if(false==check())
        return;
    statuslabel1->setText(tr("递归方式深度优先遍历"));
    ui->textBrowser_2->clear();
    memset(manager->visit, 0, sizeof(manager->visit));
    QString text, plus;
    for (int i = 0; i < manager->n; i++)
    {
        if (manager->visit[i] == 0)
        {
            text+=QString::number(i);
            manager->visit[i] = 1;
            manager->dfs_nostack(i, plus);
            text+=plus;
            plus.clear();
            text+="\n\n";
        }
    }
    ui->textBrowser_2->setText(text);
}

//非递归方式深度优先遍历
void MainWindow::on_actiondfsstack_triggered()
{
    if(false==check())
        return;
    statuslabel1->setText(tr("非递归方式深度优先遍历"));
    ui->textBrowser_2->clear();
    memset(manager->visit, 0, sizeof(manager->visit));
    manager->show.clear();
    manager->s.clear();
    QString text, plus;
    for (int i = 0; i < manager->n; i++)
    {
        if (manager->visit[i] == 0)
        {
            text+=QString::number(i);
            manager->visit[i] = 1;
            manager->dfs_stack(i, plus);
            text+=plus;
            plus.clear();
            text+="\n\n";
        }
    }
    qDebug()<<manager->show;
    for (int i = 0; i < manager->show.size(); i++)
    {
        QThread::msleep(1000);
        if(manager->show.at(i)=='i')
        {
            stackflag++;
            stackbutton[stackflag-1]->setText(QString(manager->show[i-1]));
            stackbutton[stackflag-1]->setVisible(true);
            QCoreApplication::processEvents();
        }
        else if(manager->show.at(i)=='o')
        {
            stackflag--;
            stackbutton[stackflag]->setVisible(false);
            QCoreApplication::processEvents();
        }
    }
    ui->textBrowser_2->setText(text);
}

//广度优先遍历
void MainWindow::on_actionbfs_triggered()
{
    if(false==check())
        return;
    statuslabel1->setText(tr("广度优先遍历"));
    ui->textBrowser_3->clear();
    memset(manager->visit, 0, sizeof(manager->visit));
    manager->show.clear();
    manager->q.clear();
    QString text, plus;
    for (int i = 0; i < manager->n; ++i)
    {
        if (manager->visit[i] == 0)
        {
            text+=QString::number(i);
            manager->visit[i] = 1;
            manager->bfs(i, plus);
            text+=plus;
            plus.clear();
            text+="\n\n";
        }
    }
    qDebug()<<manager->show;
    for (int i = 0; i < manager->show.size(); ++i)
    {
        QThread::msleep(1000);
        if(manager->show.at(i)=='i')
        {
            queueflag++;
            queuebutton[queueflag-1]->setText(QString(manager->show[i-1]));
            queuebutton[queueflag-1]->setVisible(true);
            QCoreApplication::processEvents();
        }
        else if(manager->show.at(i)=='o')
        {
            queueflag--;
            for(int j=0;j<queueflag;j++)
            {
                QString str= queuebutton[j+1]->text();
                queuebutton[j]->setText(str);
            }
            queuebutton[queueflag]->setVisible(false);
            QCoreApplication::processEvents();
        }
    }
    ui->textBrowser_3->setText(text);
}

//使用帮助
void MainWindow::on_actionhelp_triggered()
{
    usehelp->show();
}

//关于Qt
void MainWindow::on_actionaboutqt_triggered()
{
    QMessageBox::aboutQt(this,tr("关于Qt"));
}

//关于作者
void MainWindow::on_actionaboutauther_triggered()
{
    QMessageBox box;
    box.setWindowTitle(tr("关于作者"));
    box.setIcon(QMessageBox::Information);
    box.setText(tr("学号:1853862\n"
                   "专业:计算机科学与技术\n"
                   "联系方式:841713301@qq.com\n"));
    box.addButton(tr("确定"),QMessageBox::AcceptRole);
    if(box.exec() == QMessageBox::Accepted)
        box.close();
}
