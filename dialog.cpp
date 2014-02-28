#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QString dhtml, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->html=dhtml;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::hideEvent(QHideEvent *)
{
    emit hidden(false);
}

void Dialog::display(QString pollname, bool going, bool finished)
{
    QString htt(this->html), ht2;
    if (finished)
        ht2=htt.replace("%polling%","Le votazioni sono chiuse!");
    else if (going)
        ht2=(htt.replace("%polling%",QString("Votazioni aperte per: %1").arg(pollname)));
    else
        ht2=(htt.replace("%polling%",QString("Fra poco si vota per: %1").arg(pollname)));
    ui->label->setText(ht2);
}
