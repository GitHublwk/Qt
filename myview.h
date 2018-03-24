#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include "mybox.h"

class QTimer;

class myView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit myView(QWidget *parent=0);
    bool isColliding();
    void directionStateControl();
    void updateScore();

protected:
    void keyPressEvent(QKeyEvent *event);

signals:

public slots:
    void startGame();
    void pauseGame();
    void moveOneStep();
    void setDirection(int index);

private:
    QGraphicsScene *scene;

    QGraphicsRectItem *paintRect;

    QList<QGraphicsItem *> snake;

    QGraphicsWidget *upBtn;
    QGraphicsWidget *downBtn;
    QGraphicsWidget *rightBtn;
    QGraphicsWidget *leftBtn;
    QGraphicsWidget *startBtn;
    QGraphicsWidget *pauseBtn;
    QGraphicsWidget *restartBtn;
    QGraphicsWidget *continueBtn;

    QGraphicsTextItem *scoreText;
    QGraphicsTextItem *levelText;

    myBox *headBox;
    myBox *tailBox;
    myBox *foodBox;

    bool isStart;
    bool isPause;

    QTimer *timer;

    int direction;
};

#endif // MYVIEW_H
