#include "timer.h"

Timer::Timer(QObject *parent) :
    QTimer(parent)
{

    connect(this, SIGNAL(timeout()), this, SLOT(step()));
}

void Timer::start(){
    QTimer::start(100);
}

void Timer::step(){
    emit tick100ms();
    if (--this->count==0){
        count=10;
        emit tick1s();
    }
}
