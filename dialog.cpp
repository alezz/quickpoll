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

void Dialog::display(QString pollname, bool going, bool finished, int count)
{
    if (count==-1) this->name=pollname;
    QString htt(this->html), ht2, message;
    if (finished)
        message = QString("Le votazioni sono chiuse!");
    else if (going)
    {
        message=QString("Votazioni aperte per: %1").arg(pollname.toHtmlEscaped());
        if (count>-1) message.append(QString("<br /><small>Mancano ancora %1 secondi!</small>").arg(count));
    }
    else
        message=QString("Fra poco si vota per: %1").arg(pollname.toHtmlEscaped());
    ht2=htt.replace("%polling%",message);
    ui->label->setText(ht2);
}


void Dialog::count(int count)
{
    //si verifica sempre quando sono con le votazioni avviate e non finite
    display(this->name,true,false,count);
}
