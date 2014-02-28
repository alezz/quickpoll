#include "about.h"
#include "ui_about.h"
#include <QFile>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    QFile f(":/text/readme");
    f.open(QIODevice::ReadOnly);
    ui->text1->setText(f.readAll());
    f.close();
    f.setFileName(":/text/license");
    f.open(QIODevice::ReadOnly);
    ui->text2->setText(f.readAll());
    f.close();

}

About::~About()
{
    delete ui;
}

void About::on_pushButton_clicked()
{
    this->hide();
}
