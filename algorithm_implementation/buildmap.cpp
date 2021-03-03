#include "buildmap.h"
#include "ui_buildmap.h"

Buildmap::Buildmap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Buildmap)
{
    ui->setupUi(this);
    readfile();
    setWindowTitle("建立新图");
}

Buildmap::~Buildmap()
{
    delete ui;
}

void Buildmap::readfile()
{
    QString demoText=tr("请严格参照以下demo，否则可能会出错误\n");
    demoText+=tr("------------------------------");
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
