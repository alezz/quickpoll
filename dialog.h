#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QString dhtml, QWidget *parent = 0);
    ~Dialog();

public slots:
    void hideEvent(QHideEvent *);
    void display(QString pollname, bool going, bool finished, int count=-1);
    void count(int count);  //conto alla rovescia

signals:
    void hidden(bool);

private:
    Ui::Dialog *ui;
    QString html;
    QString name;

};

#endif // DIALOG_H
