#ifndef DYNAMICADD_H
#define DYNAMICADD_H

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QString>
#include <QColorDialog>
#include <QFile>
#include <QTextStream>

class MainWindow;

namespace Ui {
class Dynamicadd;
}

class Dynamicadd : public QDialog
{
    Q_OBJECT

private slots:
    //线路编辑内容改变
    void on_lineEditlinename_textChanged(const QString &arg1);
    //点击选择颜色按钮
    void on_pushButtoncolor_clicked();
    //站点编辑内容改变
    void on_lineEditstationname_textChanged(const QString &arg1);
    //纬度编辑内容改变
    void on_doubleSpinBoxlongitude_valueChanged(double arg1);
    //经度编辑内容改变
    void on_doubleSpinBoxlatitude_valueChanged(double arg1);
    //列表部件选择项改变
    void on_listWidget_itemClicked();

public:
    //构造函数
    explicit Dynamicadd(QWidget *parent = 0);
    //析构函数
    ~Dynamicadd();
    //读取文件
    void readfile();
    //设置默认状态
    void setdefault();
    //更新线路列表信息
    void updatelistWidget();
    //更新选择部件
    void updatecombobox();

protected:
    Ui::Dynamicadd *ui;            //UI
    QString linename;              //保存输入线路名
    QColor linecolor;              //保存输入线路颜色
    QString stationname;           //保存输入站点名
    double longitude;              //保存输入站点经度
    double latitude;               //保存输入站点纬度
    QList<QString> linenamelist;   //保存选择线路名表
    QList<QString> lineselectlist; //保存选择的线路名
    QList<QString> stationnamelist;//保存选择站点名表
    friend class MainWindow;       //声明友元
};

#endif // DYNAMICADD_H
