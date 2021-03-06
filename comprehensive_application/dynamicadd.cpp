#include "ui_dynamicadd.h"
#include "dynamicadd.h"

//构造函数
Dynamicadd::Dynamicadd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dynamicadd)
{
    ui->setupUi(this);
    readfile();
    setWindowTitle("动态添加");
}

//析构函数
Dynamicadd::~Dynamicadd()
{
    delete ui;
}

//读取文件
void Dynamicadd::readfile()
{
    QString demoText=tr("请参照下面的demo.txt\n");
    demoText+=tr("---------------------------------");
    QFile file(":/data/mydata/demo.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        demoText=file.errorString();
    }
    else
    {
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QString str=in.readLine();
            demoText+="\n";
            demoText+=str;
        }
        file.close();
        ui->textEdit->setPlaceholderText(demoText);
    }
}

//设置默认状态
void Dynamicadd::setdefault()
{
    ui->lineEditlinename->clear();
    ui->pushButtoncolor->setStyleSheet("background-color: rgb(225, 225, 225)");
    ui->lineEditstationname->clear();
    ui->doubleSpinBoxlatitude->setValue(30.9);
    ui->doubleSpinBoxlongitude->setValue(121.0);
    updatelistWidget();
}

//更新线路列表信息
void Dynamicadd::updatelistWidget()
{
    for(int i=0; i<linenamelist.size(); ++i)
    {
        delete ui->listWidget->takeItem(i);
    }
    ui->listWidget->clear();
    lineselectlist.clear();
    for(int i=0; i<linenamelist.size(); ++i)
    {
        QListWidgetItem *item=new QListWidgetItem(linenamelist[i]);
        item->setFlags(item->flags()|Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui->listWidget->addItem(item);
    }
}

//更新选择部件
void Dynamicadd::updatecombobox()
{
    ui->comboBoxstation1->clear();
    ui->comboBoxstation2->clear();
    ui->comboBoxconnectline->clear();
    for (auto &a: stationnamelist)
    {
        ui->comboBoxstation1->addItem(a);
        ui->comboBoxstation2->addItem(a);
    }
    for (auto &a: linenamelist)
    {
        ui->comboBoxconnectline->addItem(a);
    }
}

//-----------------------槽函数--------------------------//
//线路编辑内容改变
void Dynamicadd::on_lineEditlinename_textChanged(const QString &arg1)
{
    linename=arg1;
}

//路线选择颜色改变
void Dynamicadd::on_pushButtoncolor_clicked()
{
    QColorDialog colorDialog;
    colorDialog.setOptions(QColorDialog::ShowAlphaChannel);
    colorDialog.exec();
    linecolor=colorDialog.currentColor();
    QString str="background-color: #";
    str+=QString::number(linecolor.rgb()-0xff000000,16);
    ui->pushButtoncolor->setStyleSheet(str);
}

//站点编辑内容改变
void Dynamicadd::on_lineEditstationname_textChanged(const QString &arg1)
{
    stationname=arg1;
}

//经度编辑内容改变
void Dynamicadd::on_doubleSpinBoxlongitude_valueChanged(double arg1)
{
    longitude=arg1;
}

//纬度编辑内容改变
void Dynamicadd::on_doubleSpinBoxlatitude_valueChanged(double arg1)
{
     latitude=arg1;
}

//列表部件选择项改变
void Dynamicadd::on_listWidget_itemClicked()
{
    lineselectlist.clear();
    for (int i=0; i<ui->listWidget->count(); i++)
    {
        QListWidgetItem* item=ui->listWidget->item(i);
        if(item->checkState()==Qt::Checked)
        {
            lineselectlist.push_back(item->text());
        }
    }
}
