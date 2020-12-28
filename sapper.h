
#ifndef SAPPER_H
#define SAPPER_H

#include <QWidget>
#include <QMap>
#include <qlayout.h>
#include <qlcdnumber.h>
#include <QTime>
#include <QMenuBar>
#include <QMenu>
#include "PushButton.h"
#include <QMainWindow>
#include <QMessageBox>

class Sapper :public QWidget
{
    Q_OBJECT
public:
    static const uint COUNT_ROWS = 10;  //Количество строк, определены константой
    Sapper(QWidget *_pwgt = nullptr);
    ~Sapper();
private:
    enum difficylt //уровень сложности <=> количество мин
    {
        EASY = 10,
        MIDDLE = 25,
        HARD = 69,
        SPECIAL = 1
    };
    QGridLayout* ptopLayout;
    QVBoxLayout* verticalLayout;
    QHBoxLayout* horizontLayout;
    QLCDNumber* TimeInfo;
    QLCDNumber* BombInfo;
    QPushButton* ButtonInfo;
    QTimer *timer;                      //отсчет времени от начала игры
    QMap<uint,PushButton*> map_Buttons; //индексированный список кнопок-ячеек

    QMenu *bar;
    QMenu *submenu;
    QMenuBar* MenuBar;
    QMainWindow menuWin;

    QMessageBox* AboutMsg;
    QMessageBox* WinMsg;
    QMessageBox* HiscoreMsg;

    int countFlags;             // Общее количество отмеченных ячеек
    uint checkBlockFlag;        //Кол-во отмеченных полей на сетке 3х3
    int countVisibleCell;
    bool endGame;               //сообщает о конце игры
    uint *fields;               //Основной массив со значением ячеек
    uint count_cells;           //Кол-во столбцов зависит от сложности
    uint size_fields;           //кол-во ячеек count_cells * COUNT_ROWS
    uint easyScore,hardScore,middleScore; //данные с рекордами
    difficylt gameDiff;         //Текущая сложность игры

    void calcRandomBomb();
    void addBomb(uint _x);
    void calcFlags(uint _x);
    void showUnckeckFields(uint _x);
    void setSectionClear(uint _x);
    void showAllBomb();
    void createGame(uint _size);
    void winGame();             //информирует о выигрыше
    void load();
    void save();
    QPushButton* createCell(const uint& _x);
    void callbackFunc(uint _i,void(Sapper::*_func)(uint));
public slots:
    void newGame();
    void createEasyGame();
    void createHardGame();
    void createMiddleGame();
    void createSpecialGame();
    void createFields(uint _size);
    void addTime();
    void about();
    void slotButtonClicked(uint _i);
    void rightButtonClicked(uint _i);
    void middleButtonClicked(uint _i);
    void showScore();
};

#endif // SAPPER_H
