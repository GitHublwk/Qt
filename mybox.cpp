#include "mybox.h"
#include <QPainter>

myBox::myBox(const QColor &color,const QPointF &point)
    :brushColor(color)
{
    setPos(point);
}

QRectF myBox::boundingRect() const
{
    qreal penWidth=1;
    return QRectF(-15-penWidth/2,-15-penWidth/2,30+penWidth,30+penWidth);
}

void myBox::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->drawPixmap(-15,-15,30,30,QPixmap(":/images/box.gif"));
    painter->setBrush(brushColor);
    QColor penColor=brushColor;
    penColor.setAlpha(20);
    painter->setPen(penColor);
    painter->drawRect(-15,-15,30,30);
}

QPainterPath myBox::shape() const
{
    QPainterPath path;
    path.addRect(-14,-14,28,28);
    return path;
}

void myBox::setBrushColor()
{
    brushColor=Qt::green;
    update();
}

