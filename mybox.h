#ifndef MYBOX_H
#define MYBOX_H

#include <QGraphicsObject>

class myBox : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit myBox(const QColor &color=Qt::red,const QPointF &point=QPointF(0,0));
    QRectF boundingRect()const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QPainterPath shape()const;

private:
    QColor brushColor;

signals:

public slots:
};

#endif // MYBOX_H
