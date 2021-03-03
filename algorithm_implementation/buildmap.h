#ifndef BUILDMAP_H
#define BUILDMAP_H

#include<QDialog>
#include<QTabWidget>
#include<QFile>
#include<QTextStream>

class MainWindow;

namespace Ui {
class Buildmap;
}

class Buildmap : public QDialog
{
    Q_OBJECT

public:
    //构造函数
    explicit Buildmap(QWidget *parent = 0);
    //析构函数
    ~Buildmap();
    //文件读取
    void readfile();

protected:
    Ui::Buildmap *ui;
    friend class MainWindow;//声明友元
};

#endif // BUILDMAP_H
