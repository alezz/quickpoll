#ifndef TIMER_H
#define TIMER_H

#include <QTimer>

class Timer : public QTimer
{
    Q_OBJECT
public:
    explicit Timer(QObject *parent = 0);

    void start();
    void stop();

signals:

    void zero();
    void tick1s();
    void tick100ms();

public slots:
    void step();

private:
    int count;
    int seconds;

};

#endif // TIMER_H
