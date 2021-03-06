#include "usehelp.h"
#include "ui_usehelp.h"

Usehelp::Usehelp(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::Usehelp)
{
    ui->setupUi(this);
    setWindowTitle("使用帮助");
}

Usehelp::~Usehelp()
{
    delete ui;
}
