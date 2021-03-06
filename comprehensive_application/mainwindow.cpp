#include "ui_mainwindow.h"
#include "ui_dynamicadd.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene=new QGraphicsScene;
    scene->setSceneRect(0, 0, 2060, 2060);                       //确定视图大小
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);//单击场景背景并拖动鼠标时鼠标的变化
    ui->graphicsView->setMouseTracking(true);                    //启动鼠标跟踪，没有按下任何按钮也会接收鼠标移动事件
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);     //抗锯齿
    mouseaction=new Mouseaction(ui->graphicsView);
    dynamicadd=new Dynamicadd(this);
    manager=new Manager;
    usehelp=new Usehelp;
    timer=new QTimer(this);
    timer->start(1000);                                          //定时器开始计时，其中1000表示1000ms

    setWindowTitle("上海地铁换乘指南");
    loaddata();
    signal_slot();
    initstatusbar();
    transfer_line_update();
    on_actionmetronet_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mouseaction;
    delete scene;
    delete manager;
    delete dynamicadd;
    delete usehelp;
    delete timer;
}

//加载数据
void MainWindow::loaddata()
{
    bool flag = manager->readfile(":/data/mydata/rawdata.txt");
    if (false == flag)
    {
        QMessageBox box;
        box.setWindowTitle(tr("数据错误"));
        box.setIcon(QMessageBox::Warning);
        box.setText("读取数据错误!\n");
        box.addButton(tr("确定"), QMessageBox::AcceptRole);
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
    }
}

//连接信号和槽
void MainWindow::signal_slot()
{
    connect(dynamicadd->ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabWidgetupdate);
    connect(dynamicadd->ui->pushButtonline, &QPushButton::clicked, this, &MainWindow::addline);
    connect(dynamicadd->ui->pushButtonstation, &QPushButton::clicked, this, &MainWindow::addstation);
    connect(dynamicadd->ui->pushButtonconnect, &QPushButton::clicked, this, &MainWindow::addconnect);
    connect(dynamicadd->ui->pushButtontext, &QPushButton::clicked, this, &MainWindow::addtext);
    connect(ui->comboBoxstartline, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &MainWindow::transfer_startstation_update);
    connect(ui->comboBoxendline, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &MainWindow::transfer_endstation_update);
    connect(ui->pushButtonTransfer, &QPushButton::clicked, this, &MainWindow::transferinquiry);
    connect(timer, &QTimer::timeout, this, &MainWindow::timerupdate);
}

//初始化状态栏
void MainWindow::initstatusbar()
{
    statuslabel1 = new QLabel;
    statuslabel1->setMinimumSize(200,15);
    statuslabel1->setFrameShape(QFrame::StyledPanel);
    statuslabel1->setFrameShadow(QFrame::Sunken);
    statuslabel1->setText(tr("欢迎使用地铁换乘指南"));
    statuslabel2 = new QLabel;
    statuslabel2->setMinimumSize(200,15);
    statuslabel2->setFrameShape(QFrame::StyledPanel);
    statuslabel2->setFrameShadow(QFrame::Sunken);
    statuslabel2->setText(tr("0000-00-00 00:00::00 星期 "));
    ui->statusBar->addWidget(statuslabel1);
    ui->statusBar->addWidget(statuslabel2);
}

//计算混合颜色
QColor MainWindow::getmixcolor(const QList<int>& linelist)
{
    QColor finalcolor=QColor(255,255,255);
    for (int i=0; i<linelist.size(); ++i)
    {
        QColor linecolor=manager->getlinecolor(linelist[i]);
        finalcolor.setRed(finalcolor.red()*linecolor.red()/255);
        finalcolor.setGreen(finalcolor.green()*linecolor.green()/255);
        finalcolor.setBlue(finalcolor.blue()*linecolor.blue()/255);
    }
    return finalcolor;
}

//获得线路表的名字集
QString MainWindow::getlinenames(const QList<int>& linelist)
{
    QString str;
    for (int i=0; i<linelist.size(); ++i)
    {
        str+=manager->getlinename(linelist[i]);
        str+=" ";
    }
    return str;
}

//将站点的经纬度地理坐标转为视图坐标
QPointF MainWindow::toviewcoord(QPointF coord)
{
    double x=(coord.x()-manager->getmincoord().x())/(manager->getmaxcoord().x()-manager->getmincoord().x())*2000+30;
    double y=(manager->getmaxcoord().y()-coord.y())/(manager->getmaxcoord().y()-manager->getmincoord().y())*2000+30;
    return QPointF(x,y);
}

//绘制网络图的边
void MainWindow::drawline(const QList<Edge>& edgelist)
{
    for(int i=0; i<edgelist.size(); ++i)
    {
        QList<int> linesList=manager->getcommonlines(edgelist[i].first, edgelist[i].second);
        QString tooltip="途经："+manager->getstationname(edgelist[i].first)+"-"+manager->getstationname(edgelist[i].second)+"\n"
                        "线路："+getlinenames(linesList);
        QPointF startpos=toviewcoord(manager->getstationcoord(edgelist[i].first));
        QPointF endpos=toviewcoord(manager->getstationcoord(edgelist[i].second));
        QGraphicsLineItem* lineitem=new QGraphicsLineItem;
        lineitem->setPen(QPen(getmixcolor(linesList), 2));
        lineitem->setCursor(Qt::PointingHandCursor);
        lineitem->setToolTip(tooltip);
        lineitem->setPos(startpos);
        lineitem->setLine(0, 0, endpos.x()-startpos.x(), endpos.y()-startpos.y());
        scene->addItem(lineitem);
    }
}

//绘制网络图的站点节点
void MainWindow::drawstation(const QList<int>& stationlist)
{
    for (int i=0; i<stationlist.size(); ++i)
    {
        QString name=manager->getstationname(stationlist[i]);
        QList<int> linesList=manager->getstationlines(stationlist[i]);
        QColor color=getmixcolor(linesList);
        QPointF longiLati=manager->getstationcoord(stationlist[i]);
        QPointF coord=toviewcoord(longiLati);
        QString tooltip="站名："+name+"\n"+
                        "经度："+QString::number(longiLati.x(),'f',7)+"\n"+
                        "纬度："+QString::number(longiLati.y(),'f',7)+"\n"+
                        "线路："+getlinenames(linesList);
        QGraphicsEllipseItem* stationitem=new QGraphicsEllipseItem;
        stationitem->setPen(color);
        stationitem->setCursor(Qt::PointingHandCursor);
        stationitem->setToolTip(tooltip);
        stationitem->setPos(coord);
        stationitem->setRect(-3, -3, 6, 6);
        scene->addItem(stationitem);
        QGraphicsTextItem* textitem=new QGraphicsTextItem;
        textitem->setPlainText(name);
        textitem->setFont(QFont("consolas",3,1));
        textitem->setPos(coord.x(),coord.y()-3*2);
        scene->addItem(textitem);
    }
}

//----------------------槽函数----------------------//
//时间更新
void MainWindow::timerupdate()
{
    QDateTime time=QDateTime::currentDateTime();
    statuslabel2->setText(time.toString("yyyy-MM-dd hh:mm:ss dddd"));
}

//换乘路线更新
void MainWindow::transfer_line_update()
{
    ui->comboBoxstartline->clear();
    ui->comboBoxendline->clear();
    for(auto &a:manager->getlinenamelist())
    {
        ui->comboBoxstartline->addItem(a);
        ui->comboBoxendline->addItem(a);
    }
    transfer_startstation_update(ui->comboBoxstartline->itemText(0));
    transfer_endstation_update(ui->comboBoxendline->itemText(0));
}

//换乘起始站点更新
void MainWindow::transfer_startstation_update(const QString& linename)
{
    ui->comboBoxstartstation->clear();
    int linehash=manager->getlinehash(linename);
    if(linehash==-1)
    {
        return;
    }
    for(auto &a:manager->getstationlist(linehash))
    {
        ui->comboBoxstartstation->addItem(a);
    }
}

//换乘终点站更新
void MainWindow::transfer_endstation_update(const QString& linename)
{
    ui->comboBoxendstation->clear();
    int linehash=manager->getlinehash(linename);
    if(linehash==-1)
    {
        return ;
    }
    for(auto &a:manager->getstationlist(linehash))
    {
        ui->comboBoxendstation->addItem(a);
    }
}

//换乘查询
void MainWindow::transferinquiry()
{
    int s1=manager->getstationhash(ui->comboBoxstartstation->currentText());
    int s2=manager->getstationhash(ui->comboBoxendstation->currentText());
    bool way=ui->radioButtonmintime->isChecked();
    if(s1==-1||s2==-1)
    {
        QMessageBox box;
        box.setWindowTitle(tr("查询错误"));
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("线路没有"));
        box.addButton(tr("确定"),QMessageBox::AcceptRole);
        if(box.exec()==QMessageBox::Accepted)
        {
            box.close();
        }
    }
    else
    {
        QList<int> stationlist;
        QList<Edge> linelist;
        bool flag;
        if(true==way)
        {
            flag=manager->getmintimetransfer(s1, s2, stationlist, linelist);
        }
        else
        {
            flag=manager->getmintransferline(s1, s2, stationlist, linelist);
        }
        if(flag)
        {
            statuslabel1->setText(tr("换乘查询成功"));
            scene->clear();
            drawline(linelist);
            drawstation(stationlist);
            QString text=way?("以下线路时间最短\n\n"):("以下线路换乘最少\n\n");
            for(int i=0; i<stationlist.size(); ++i)
            {
                text+=manager->getstationname(stationlist[i]);
                if(i!=stationlist.size()-1)
                text+="\n ↓\n";
            }
            ui->textBrowser->clear();
            ui->textBrowser->setText(text);
        }
        else
        {
            QMessageBox box;
            box.setWindowTitle(tr("查询错误"));
            box.setIcon(QMessageBox::Warning);
            box.setText(tr("换乘无法到达终点站"));
            box.addButton(tr("确定"),QMessageBox::AcceptRole);
            if(box.exec()==QMessageBox::Accepted)
            {
                box.close();
            }
        }
    }
}

//动态添加更新
void MainWindow::tabWidgetupdate(int)
{
    QWidget* widget=dynamicadd->ui->tabWidget->currentWidget();
    if(widget==dynamicadd->ui->tabstation)
    {
        dynamicadd->linenamelist=manager->getlinenamelist();
        dynamicadd->updatelistWidget();
    }
    else if(widget==dynamicadd->ui->tabconnect)
    {
        dynamicadd->linenamelist=manager->getlinenamelist();
        dynamicadd->stationnamelist=manager->getstationnamelist();
        dynamicadd->ui->comboBoxstation1->setMaxCount(dynamicadd->stationnamelist.size());
        dynamicadd->ui->comboBoxstation2->setMaxCount(dynamicadd->stationnamelist.size());
        dynamicadd->ui->comboBoxconnectline->setMaxCount(dynamicadd->linenamelist.size());
        dynamicadd->updatecombobox();
    }
}

//添加线路功能函数
void MainWindow::addline()
{
    QMessageBox box;
    box.setWindowTitle(tr("添加线路"));
    if(dynamicadd->linename.isEmpty())
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("请输入线路名称"));
    }
    else if(manager->getlinehash(dynamicadd->linename)!=-1)
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("线路名已存在"));
    }
    else
    {
        box.setIcon(QMessageBox::Information);
        box.setText(tr("线路：")+dynamicadd->linename+tr("添加成功"));
        manager->addline(dynamicadd->linename, dynamicadd->linecolor);
        transfer_line_update();
    }
    box.addButton(tr("确定"),QMessageBox::AcceptRole);
    if(box.exec()==QMessageBox::Accepted)
    {
        box.close();
    }
}

//添加站点功能函数
void MainWindow::addstation()
{
    QMessageBox box;
    box.setWindowTitle(tr("添加站点"));
    if(dynamicadd->stationname.isEmpty())
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("请输入站点名称"));
    }
    else if(dynamicadd->lineselectlist.isEmpty())
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("请选择站点所属线路"));
    }
    else if(manager->getstationhash(dynamicadd->stationname)!=-1)
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("站点已存在"));
    }
    else
    {
        Station s(dynamicadd->stationname, dynamicadd->longitude, dynamicadd->latitude, manager->getlinehashlist(dynamicadd->lineselectlist));
        manager->addstation(s);
        box.setText(tr("站点：")+dynamicadd->stationname+tr("添加成功"));
        transfer_line_update();
    }
    box.addButton(tr("确定"),QMessageBox::AcceptRole);
    if(box.exec()==QMessageBox::Accepted)
    {
        box.close();
    }
    on_actionmetronet_triggered();
}

//添加连接功能函数
void MainWindow::addconnect()
{
    QMessageBox box;
    box.setWindowTitle(tr("添加连接"));
    QString station1=dynamicadd->ui->comboBoxstation1->currentText();
    QString station2=dynamicadd->ui->comboBoxstation2->currentText();
    int s1=manager->getstationhash(station1);
    int s2=manager->getstationhash(station2);
    int l=manager->getlinehash(dynamicadd->ui->comboBoxconnectline->currentText());
    if(s1==-1||s2==-1||l==-1)
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("请选择已有的站点和线路"));
    }
    else if(s1==s2)
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("同一站点不需要连接"));
    }
    else if(!manager->getstationlines(s1).contains(l))
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("所属线路不包含站点1"));
    }
    else if(!manager->getstationlines(s2).contains(l))
    {
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("所属线路不包含站点2"));
    }
    else
    {
        box.setIcon(QMessageBox::Information);
        box.setText(tr("添加连接成功"));
        manager->addconnect(s1,s2,l);
        transfer_line_update();
    }
    if(box.exec()==QMessageBox::Accepted)
    {
        box.close();
    }
    on_actionmetronet_triggered();
}

//文本方式添加功能函数
void MainWindow::addtext()
{
    QMessageBox box;
    box.setWindowTitle(tr("文本添加"));
    QString filename="addtext.txt";
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        box.setIcon(QMessageBox::Warning);
        box.setText("无法创建添加文件");
        return;
    }
    QTextStream out(&file);
    out<<dynamicadd->ui->textEdit->toPlainText();
    file.close();
    bool flag=manager->readfile(filename);
    if(flag)
    {
        box.setIcon(QMessageBox::Information);
        box.setText(tr("添加成功"));
        transfer_line_update();
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
    on_actionmetronet_triggered();
}

//视图放大
void MainWindow::on_actionenlarge_triggered()
{
    statuslabel1->setText(tr("视图放大"));
    ui->graphicsView->scale(1.25, 1.25);
}

//视图缩小
void MainWindow::on_actionnarrow_triggered()
{
    statuslabel1->setText(tr("视图缩小"));
    ui->graphicsView->scale(0.8, 0.8);
}

//动态添加
void MainWindow::on_actionadd_triggered()
{
    statuslabel1->setText(tr("动态添加"));
    dynamicadd->setdefault();
    dynamicadd->show();
}

//地铁线路总图
void MainWindow::on_actionmetronet_triggered()
{
    statuslabel1->setText(tr("上海地铁线路图"));
    scene->clear();
    QList<int> stationsList;
    QList<Edge> edgesList;
    manager->getgeneralroadmap(stationsList,edgesList);
    drawline(edgesList);
    drawstation(stationsList);
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
void MainWindow::on_actionaboutauthor_triggered()
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
