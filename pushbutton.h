#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include <qpushbutton.h>
#include <QtGui>

#define sizeButton 25

class PushButton:public QPushButton
{
    Q_OBJECT
public:
    PushButton(uint _i = 0,QPushButton* qpb = nullptr,const QString &_str = "");
    uint getIndex();
    void setIndex(uint _i);
    uint getIconIndex();
    void clear();
    bool isFlag();
private:
    uint ind;
    uint IconIndex;
    void mousePressEvent(QMouseEvent *event);
signals:
    void left_clicked(uint);
    void right_clicked(uint);
    void middle_clicked(uint);
};

#endif // PUSHBUTTON_H
