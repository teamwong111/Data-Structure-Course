#ifndef MOUSEACTION_H
#define MOUSEACTION_H

#include <QObject>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <qmath.h>

class Mouseaction : public QObject
{
    Q_OBJECT

public:
    //构造函数
    Mouseaction(QGraphicsView*);

private:
    //事件过滤器
    bool eventFilter(QObject*, QEvent*);

    QGraphicsView* myview;
    QPointF scenepos, viewpos;
};

#endif // MOUSEACTION_H
