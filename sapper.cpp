#include "Sapper.h"

//колбек ф-ция выполняет ф-цию (* _func) для ячеек 3х3 (соседние ячейки значения _и), которые выходят за пределы поля
void Sapper::callbackFunc(uint _i,void(Sapper::*_func)(uint))
{
    uint endY,endX,startY,startX,i,j;
    endX = startX = _i / count_cells;
    if (startX > 0) startX--;
        endX+= 2;
    if (endX > COUNT_ROWS)endX--;

    endY = startY = _i % count_cells;
    if (startY > 0) startY--;
    endY+= 2;

    if (endY > count_cells)endY--;

    for(i = startX; i<endX; ++i)
        for(j = startY; j<endY;++j)
             (this->*_func)(i*count_cells+j);
}
//Инициализация всех виджетов и их размещение и тд.
Sapper::Sapper(QWidget *_pwgt):QWidget(_pwgt,Qt::WindowMinimizeButtonHint){

    ptopLayout = new QGridLayout;
    horizontLayout = new QHBoxLayout;
    verticalLayout = new QVBoxLayout;
    TimeInfo = new QLCDNumber(3);
    BombInfo = new QLCDNumber(3);
    ButtonInfo = new QPushButton("");
    timer = new QTimer(this);
    AboutMsg = new QMessageBox(QMessageBox::Information,"O QtSapper","Qt Sapper v1.0<p>Copyright © 2019 Titarenko Aleksey.",QMessageBox::Ok);
    WinMsg = new QMessageBox(QMessageBox::Information,"You win","You score:",QMessageBox::Ok);
    HiscoreMsg = new QMessageBox(QMessageBox::NoIcon,"Score Info","You score:",QMessageBox::Ok);

    ButtonInfo->setIcon(QPixmap(":/icons/bad.png"));
    ButtonInfo->setMinimumSize(40,40);
    ButtonInfo->setMaximumSize(40,40);
    BombInfo->setMinimumHeight(40);
    TimeInfo->setMinimumHeight(40);
    TimeInfo->display(0);

    load();

    connect(ButtonInfo,SIGNAL(clicked()),SLOT(newGame()));//конектим кнопку со "смайликом"
    connect(timer,SIGNAL(timeout()),SLOT(addTime()));

//Меню
    MenuBar = menuWin.menuBar();
    bar = MenuBar->addMenu("&File");
    bar->addAction("New",this,SLOT(newGame()),Qt::CTRL+Qt::Key_N);
    bar->addSeparator();

    submenu = bar->addMenu("&Difficulty");
    submenu->addAction("Easy", this,SLOT(createEasyGame()));
    submenu->addAction("Middle", this,SLOT(createMiddleGame()));
    submenu->addAction("Hard", this,SLOT(createHardGame()));
    QMenu *submenu1 = submenu->addMenu("&Special");
    submenu1->addAction("Random Bombs and Size", this,SLOT(createSpecialGame()));

    bar->addAction("Hiscore", this,SLOT(showScore()));
    bar->addSeparator();
    bar->addAction("Exit", this,SLOT(close()));

    bar = MenuBar->addMenu("&Help");
    bar->addAction("About",this,SLOT(about()));


//Верхние виджеты добавлено по горизонтальному выравниванию
    horizontLayout->setMargin(10);
    horizontLayout->addWidget(TimeInfo);
    horizontLayout->addWidget(ButtonInfo);
    horizontLayout->addWidget(BombInfo);

    endGame = false;
    ptopLayout->setMargin(0);
    ptopLayout->setSpacing(0);

    countFlags = gameDiff = EASY;
    createFields(10);
   // createEasyGame();
//Основной слой в который добавлено инф. виджеты и вджт поля
    verticalLayout->setMargin(0);
    verticalLayout->addWidget(MenuBar);
    verticalLayout->addLayout(horizontLayout);

    verticalLayout->addLayout(ptopLayout);
    setLayout(verticalLayout);

    BombInfo->display(countFlags);
}
//Перезапуск игры
void Sapper::newGame()
{
    ButtonInfo->setIcon(QPixmap(":/icons/smile.png"));
    endGame = false;
    TimeInfo->display(0);
    countFlags = gameDiff;
    BombInfo->display(countFlags);
    timer->stop();
    countVisibleCell = 0;
    for(uint k = 0; k < size_fields; ++k)
    {
        fields[k] = 0;
        map_Buttons[k]->clear();
    }
    calcRandomBomb();
}
void Sapper::about()
{
    AboutMsg->show();
}


void Sapper::createEasyGame()
{
    if (gameDiff != EASY) createGame(10);
    countFlags =  gameDiff = EASY;
    setFixedSize(10*sizeButton,325);
    newGame();
}

void Sapper::createMiddleGame()
{
    if (gameDiff != MIDDLE) createGame(15);
    countFlags = gameDiff = MIDDLE;
    setFixedSize(15*sizeButton,325);
    newGame();
}

void Sapper::createSpecialGame()
{
    uint Cg = qrand() % 20 + 10;
    createGame(Cg);
    countFlags = gameDiff = SPECIAL;
    setFixedSize(int(Cg*sizeButton),325);
    newGame();
}

void Sapper::createHardGame()
{
    if (gameDiff != HARD) createGame(30);
    countFlags = gameDiff = HARD;
    setFixedSize(30*sizeButton,325);
    newGame();
}

void Sapper::createGame(uint _size)
{
    BombInfo->display(countFlags);
    for(uint k = 0; k < size_fields; ++k)
    {
        disconnect(map_Buttons[k]);
        delete map_Buttons[k];
    };
    map_Buttons.clear();
    delete[] fields;
    createFields(_size);
}

//создаем новое поле с виджетами
void Sapper::createFields(uint _size)
{
    count_cells = _size;
    countVisibleCell = 0;
    size_fields = count_cells*COUNT_ROWS;
    fields = new uint[size_fields];
        for(uint k = 0; k < size_fields; ++k)
        {
            ptopLayout->addWidget(createCell(k),k/count_cells,k%count_cells);
            fields[k] = 0;
        }
     calcRandomBomb();
}

//добавляем виджет
QPushButton* Sapper::createCell(const uint &_x)
{
    PushButton *ppb = new PushButton(_x,0,""/*QString().number(fields[_x])*/);
    map_Buttons[_x] = ppb;
    connect(ppb,SIGNAL(left_clicked(uint)),SLOT(slotButtonClicked(uint)));
    connect(ppb,SIGNAL(right_clicked(uint)),SLOT(rightButtonClicked(uint)));
    connect(ppb,SIGNAL(middle_clicked(uint)),SLOT(middleButtonClicked(uint)));
    return ppb;
}

//Открываем соседние ячейки если это возможно
void Sapper::middleButtonClicked(uint _i)
{
    if(endGame) return;
    if(fields[_i] == 0)return;
    checkBlockFlag = 0;
    callbackFunc(_i,&Sapper::calcFlags);
    if (checkBlockFlag == fields[_i])
        callbackFunc(_i,&Sapper::showUnckeckFields);
}

void Sapper::showUnckeckFields(uint _x)
{
    if(!map_Buttons[_x]->isFlag())
        slotButtonClicked( _x);
}

void Sapper::calcFlags(uint _x)
{
    if (map_Buttons[_x]->isFlag())
        ++checkBlockFlag;
}

void Sapper::rightButtonClicked(uint _i)
{
    if(endGame) return;
    switch (map_Buttons[_i]->getIconIndex()){
     case 0:
        map_Buttons[_i]->setIcon(QPixmap(""));
        ++countFlags;
        break;
     case 1:
        map_Buttons[_i]->setIcon(QPixmap(":/icons/check.png"));
        --countFlags;
        break;
     case 2:
        map_Buttons[_i]->setIcon(QPixmap(":/icons/query.png"));
        break;
    }
    BombInfo->display(countFlags);
}

void Sapper::slotButtonClicked(uint _i)
{
    if(endGame) return;
    if(!timer->isActive()) timer->start(1000);
    if((_i < 0)||(_i > size_fields)) return;
    if (map_Buttons[_i]->isFlat()||(map_Buttons[_i]->getIconIndex() != 0)) return;

    if(fields[_i] < size_fields )
    {
        map_Buttons[_i]->setFlat(true);
        ++countVisibleCell;
        if(fields[_i] > 0)      //устанавливаем номер иконки
            map_Buttons[_i]->setIcon(QPixmap(":/icons/"+QString().number(fields[_i])+".png"));

       if (fields[_i] == 0 )
            callbackFunc(_i,&Sapper::slotButtonClicked);
    };
    if(fields[_i] >= size_fields )//Попали на мину
    {
        endGame = true;
        ButtonInfo->setIcon(QPixmap(":/icons/bad.png"));
        timer->stop();
        showAllBomb();
    };
    if(countVisibleCell >= (size_fields-gameDiff))//Все ячейки открыты
            winGame();//победа
}

void Sapper::winGame()
{
    timer->stop();
    uint value = TimeInfo->intValue();
    bool newValue = false;
    WinMsg->setText("You win score: " + QString().number(TimeInfo->intValue()));
    switch(gameDiff)
    {
        case EASY:
            if(value < easyScore)
            {
                easyScore = value;
                newValue = true;
            }
            break;
        case MIDDLE:
            if(value < middleScore)
            {
                middleScore = value;
                newValue = true;
            }
            break;
        case HARD:
            if(value < hardScore)
            {
                hardScore = value;
                newValue = true;
            }
            break;
    };
    if(newValue)
        save();
    WinMsg->show();
}
//Сообщение с рекордами
void Sapper::showScore()
{
    HiscoreMsg->setText("<table><tr><td><b/>Difficulty</td><td> | </td><td><b/>Time</td></tr>"
                        "<tr><td>Easy</td><td> | </td><td>"+QString().number(easyScore)+"</td></tr>"
                        "<tr><td>Middle</td><td> | </td><td>"+QString().number(middleScore)+"</td></tr>"
                        "<tr><td>Hard</td><td> | </td><td>"+QString().number(hardScore)+"</td><tr></table>");
    HiscoreMsg->show();
}

//Открываем размещения всех мин
void Sapper::showAllBomb()
{
    for(uint k = 0; k < size_fields; ++k)
        if(fields[k] >= size_fields)
        {
            if(map_Buttons[k]->isFlag()) continue;
            map_Buttons[k]->setFlat(true);
            map_Buttons[k]->setIcon(QPixmap(":/icons/bomb.png"));
        }
        else
            if(map_Buttons[k]->isFlag())
            {
                map_Buttons[k]->setFlat(true);
                map_Buttons[k]->setIcon(QPixmap(":/icons/bad_check.png"));
            }
}

//расстановка бомб
void Sapper::calcRandomBomb()
{
    uint countBomb = 0;
    uint number = 0;
    uint randomnum = 0;
    qsrand(static_cast<uint>(QTime::currentTime().msec()));

    if (gameDiff == 1)
    {
        randomnum = qrand() % (size_fields + 1);
    }

        while((countBomb < gameDiff) || (countBomb < randomnum))
        {
            number = qrand() % (size_fields + 1);

            if(fields[number] < size_fields)
            {
                fields[number] = size_fields + countBomb;
                ++countBomb;
                callbackFunc(number,&Sapper::addBomb);
            }
        }

}


void Sapper::addBomb(uint _x)
{
    if (_x < size_fields)
            if(fields[_x] < size_fields)
                ++fields[_x];
}
//отсчет времени
void Sapper::addTime()
{
   TimeInfo->display(TimeInfo->intValue()+1);
}

void Sapper::load()
{
    QFile file("score.dat");
    easyScore = middleScore = hardScore = 999;
    if (file.open(QFile::ReadOnly))
    {
        QDataStream in(&file);
        in >> easyScore >> middleScore >> hardScore;
    }
    file.close();
}

void Sapper::save()
{
    QFile file("score.dat");
    if(file.open(QFile::WriteOnly))
    {
        QDataStream out(&file);
        out<< easyScore << middleScore << hardScore;
    }
    file.close();
}

//очистка
Sapper::~Sapper()
{
  for(uint k = 0; k < size_fields; ++k)
    delete map_Buttons[k];

  map_Buttons.clear();

  delete  ptopLayout;
  delete  horizontLayout;
  delete  verticalLayout;
  delete  AboutMsg;
  delete  TimeInfo;
  delete  BombInfo;
  delete  ButtonInfo;
  delete  timer;
  delete[] fields;

}

