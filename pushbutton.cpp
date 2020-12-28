#include "PushButton.h"

PushButton::PushButton(uint _i, QPushButton *_qpb,const QString &_str):QPushButton(_qpb)
{
    ind = _i;
    setMinimumSize(sizeButton,sizeButton);
    setMaximumSize(sizeButton,sizeButton);
    setText(_str);
    IconIndex = 0;
}
void PushButton::setIndex(const uint _i)
{
        ind = _i;
}
uint PushButton::getIndex()
{
    return ind;
}
bool PushButton::isFlag()
{
    return (IconIndex == 1)?true:false;
}

uint PushButton::getIconIndex()
{
    return IconIndex;
}
void PushButton::clear()
{
    setFlat(false);
    IconIndex = 0;
    setIcon(QIcon());
}

void PushButton::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
        case Qt::RightButton:
             IconIndex++;
             if (IconIndex>2)IconIndex-= 3;
             if(!isFlat())emit right_clicked(ind);
         //    if(!isFlat()) qDebug() << ind << endl;
             break;
         case Qt::MiddleButton:
             emit middle_clicked(ind);
             break;
         default:
            if(IconIndex == 0)emit left_clicked(ind);
            break;
    }
}
