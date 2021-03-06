#include "mouseaction.h"

Mouseaction::Mouseaction(QGraphicsView* view)
    : QObject(view), myview(view)
{
    //安装事件过滤器
    myview->viewport()->installEventFilter(this);
}

bool Mouseaction::eventFilter(QObject *object, QEvent *event)
{
    //鼠标移动获取viewpos和scenepos
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mevent = static_cast<QMouseEvent*>(event);
        viewpos = mevent->pos();
        scenepos = myview->mapToScene(mevent->pos());
    }
    //滚轮移动完成缩放
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent* wevent = static_cast<QWheelEvent*>(event);
        if (QApplication::keyboardModifiers() == Qt::NoModifier && wevent->orientation() == Qt::Vertical)
        {
            double scalex = qPow(1.0015, wevent->angleDelta().y());
            myview->scale(scalex, scalex);
            QPointF delta = myview->mapFromScene(scenepos) - viewpos;
            QPointF viewcenter = delta + QPointF(myview->viewport()->width() / 2.0,
                                                 myview->viewport()->height() / 2.0);;
            myview->centerOn(myview->mapToScene(viewcenter.toPoint()));
            return true;
        }
    }
    //表明未使用，消除警告
    Q_UNUSED(object)
    return false;
}
