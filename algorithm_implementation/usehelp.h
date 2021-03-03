#ifndef USEHELP_H
#define USEHELP_H

#include<QTabWidget>

namespace Ui {
class Usehelp;
}

class Usehelp : public QTabWidget
{
    Q_OBJECT

public:
    explicit Usehelp(QWidget *parent = 0);
    ~Usehelp();

private:
    Ui::Usehelp *ui;
};

#endif // USEHELP_H
