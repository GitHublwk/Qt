#include "myview.h"
#include <QLabel>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QPushButton>
#include <QButtonGroup>
#include <QGraphicsProxyWidget>
#include <QGraphicsItem>
#include <QTime>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

static const int SPEED=1000;

myView::myView(QWidget *parent)
    : QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing);
    setCacheMode(CacheBackground);
    setMaximumSize(810,525);
    setMinimumSize(810,525);
    setWindowTitle(tr("贪吃蛇"));
    setWindowIcon(QIcon(":/images/icon.ico"));

    scene=new QGraphicsScene();
    scene->setSceneRect(QRectF(0,0,800,510));
    scene->setBackgroundBrush(QPixmap(":/images/background.jpg"));
    setScene(scene);

    paintRect=scene->addRect(0,0,630,510);

    for(int i=1;i<21;i++)
    {
        QGraphicsLineItem *line=scene->addLine(30*i,0,30*i,510);
    }
    for(int i=1;i<17;i++)
    {
        QGraphicsLineItem *line=scene->addLine(0,30*i,630,30*i);
    }

    QGraphicsTextItem *scoreLabel=scene->addText(tr("得分："));
    scoreLabel->setFont(QFont("宋体",12));
    scoreText=new QGraphicsTextItem(0);
    scoreText=scene->addText(tr("0"));
    scoreText->setFont(QFont("宋体",12));
    levelText=new QGraphicsTextItem(0);
    levelText=scene->addText(tr("等级：1级"));
    levelText->setFont(QFont("宋体",12));

    scoreLabel->setPos(690,100);
    scoreText->setPos(740,100);
    levelText->setPos(690,150);

    QPushButton *btn1=new QPushButton(QIcon(tr(":/images/up.png")),0,0);
    btn1->setShortcut(Qt::Key_W);
    QPushButton *btn2=new QPushButton(QIcon(tr(":/images/down.png")),0,0);
    btn2->setShortcut(Qt::Key_S);
    QPushButton *btn3=new QPushButton(QIcon(tr(":/images/right.png")),0,0);
    btn3->setShortcut(Qt::Key_D);
    QPushButton *btn4=new QPushButton(QIcon(tr(":/images/left.png")),0,0);
    btn4->setShortcut(Qt::Key_A);

    QButtonGroup *btnGroup=new QButtonGroup();
    btnGroup->addButton(btn1,0);
    btnGroup->addButton(btn2,2);
    btnGroup->addButton(btn3,1);
    btnGroup->addButton(btn4,3);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(setDirection(int)));

    QPushButton *btn5=new QPushButton(tr("Start"));
    btn5->setFont(QFont("宋体",12));
    QPushButton *btn6=new QPushButton(tr("Pause"));
    btn5->setFont(QFont("宋体",12));
    QPushButton *btn7=new QPushButton(tr("Restart"));
    btn5->setFont(QFont("宋体",12));
    QPushButton *btn8=new QPushButton(tr("Continue"));
    btn5->setFont(QFont("宋体",12));

    connect(btn5,SIGNAL(clicked()),this,SLOT(startGame()));
    connect(btn6,SIGNAL(clicked()),this,SLOT(pauseGame()));
    connect(btn7,SIGNAL(clicked()),this,SLOT(startGame()));
    connect(btn8,SIGNAL(clicked()),this,SLOT(pauseGame()));

    upBtn=scene->addWidget(btn1);
    downBtn=scene->addWidget(btn2);
    rightBtn=scene->addWidget(btn3);
    leftBtn=scene->addWidget(btn4);
    startBtn=scene->addWidget(btn5);
    pauseBtn=scene->addWidget(btn6);
    restartBtn=scene->addWidget(btn7);
    continueBtn=scene->addWidget(btn8);

    upBtn->setPos(707,200);
    downBtn->setPos(707,300);
    rightBtn->setPos(742,250);
    leftBtn->setPos(671,250);
    startBtn->setPos(641,350);
    pauseBtn->setPos(725,350);
    restartBtn->setPos(641,350);
    continueBtn->setPos(725,350);

    restartBtn->hide();
    continueBtn->hide();

    isStart=false;
    isPause=false;

    pauseBtn->setEnabled(false);

    timer=new QTimer();
    timer->setInterval(SPEED);
    connect(timer,SIGNAL(timeout()),this,SLOT(moveOneStep()));

    qsrand(static_cast<unsigned>( time(NULL)));
}

bool myView::isColliding()
{
    foreach(QGraphicsItem *item,snake){
        if(!paintRect->contains(item->pos()))
            return true;
    }
    for (int i=1;i<snake.size()-1;i++)
    {
        if(snake.first()->pos()==snake.at(i)->pos())
            return true;
    }
    return false;
}

void myView::directionStateControl()
{
    switch(direction)
    {
    case 0:
        downBtn->setEnabled(false);
        rightBtn->setEnabled(true);
        leftBtn->setEnabled(true);
        break;
    case 1:
        leftBtn->setEnabled(false);
        upBtn->setEnabled(true);
        downBtn->setEnabled(true);
        break;
    case 2:
        upBtn->setEnabled(false);
        rightBtn->setEnabled(true);
        leftBtn->setEnabled(true);
        break;
    case 3:
        rightBtn->setEnabled(false);
        upBtn->setEnabled(true);
        downBtn->setEnabled(true);
        break;
    }
}

void myView::updateScore()
{
    int score=scoreText->toPlainText().toInt();
    score+=1;
    scoreText->setPlainText(tr("%1").arg(score));
    levelText->setPlainText(tr("等级：%1级").arg(score/5+1));

    timer->start(score<25?(1000-score/5*200):50/(score-24));
}

void myView::setDirection(int index)
{
    switch (index)
    {
    case 0:
        direction=0;
        break;
    case 1:
        direction=1;
        break;
    case 2:
        direction=2;
        break;
    case 3:
        direction=3;
        break;
    }
    directionStateControl();
}

void myView::keyPressEvent(QKeyEvent *event)
{
    if(isStart)
    {
        switch(event->key()){
        case Qt::Key_W:
            if(direction!=2)
                direction=0;
            break;
        case Qt::Key_D:
            if(direction!=3)
                direction=1;
            break;
        case Qt::Key_S:
            if(direction!=0)
                direction=2;
            break;
        case Qt::Key_A:
            if(direction!=1)
                direction=3;
            break;
        }

        directionStateControl();
    }
}

void myView::startGame()
{
    while(isStart)
    {
        foreach(QGraphicsItem *item,snake)
        {
            item=snake.first();
            myBox *tempBox=static_cast<myBox *>(item);
            snake.removeFirst();
            tempBox->deleteLater();
        }
        foodBox->deleteLater();

        startBtn->show();
        restartBtn->hide();

        isStart=false;

        scoreText->setPlainText(tr("0"));
        levelText->setPlainText(tr("等级：1级"));
    }

    do{   
        foodBox=new myBox(Qt::red,QPoint((qrand()%20)*30+15,(qrand()%16)*30+15));
        headBox=new myBox(Qt::green,QPoint((qrand()%20)*30+15,(qrand()%16)*30+15));        
    }while(foodBox->pos()==headBox->pos());

    direction=qrand()%3;

    scene->addItem(foodBox);
    scene->addItem(headBox);
    snake.append(headBox);

    directionStateControl();

    startBtn->hide();
    restartBtn->show();

    pauseBtn->setEnabled(true);

    isStart=true;

    timer->start();
}

void myView::pauseGame()
{
    if(isPause==false)
    {
        timer->stop();

        pauseBtn->hide();
        continueBtn->show();
        isPause=true;
    }
    else
    {
        timer->start();

        pauseBtn->show();
        continueBtn->hide();
        isPause=false;
    }
}

void myView::moveOneStep()
{
    QPointF point=snake.at(0)->pos();

    switch(direction){
    case 0:
        point.ry()-=30;        
        break;
    case 1:
        point.rx()+=30;        
        break;
    case 2:
        point.ry()+=30;        
        break;
    case 3:
        point.rx()-=30;        
        break;
    }

    headBox=new myBox(Qt::green,point);
    snake.prepend(headBox);

    while(isColliding())
    {
        if(QMessageBox::Yes==QMessageBox::information(this,tr("Game Over"),tr("本次得分：%1!").arg(scoreText->toPlainText().toInt()),QMessageBox::Yes))
        {
            timer->stop();

            pauseBtn->setEnabled(false);

            return;
        }
    }
    scene->addItem(headBox);

    tailBox=static_cast<myBox *>(snake.last());

    if(snake.at(0)->pos()==foodBox->pos())
    {
        do{
            foodBox->deleteLater();
            foodBox=new myBox(Qt::red,QPoint((qrand()%20)*30+15,(qrand()%16)*30+15));

        }while(isColliding());

        updateScore();

        scene->addItem(foodBox);
        return;
    }

    snake.removeLast();
    tailBox->deleteLater();
}
