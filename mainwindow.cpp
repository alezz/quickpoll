//This file is part of Quickpoll.

//Author: Mauro Alessandro (www.maetech.it) (c) march 2014

//Quickpoll is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Foobar is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Foobar.  If not, see <http://www.gnu.org/licenses/>.


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QStandardItemModel>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>

#include <QDebug>

#define SETTINGS_FILENAME "../quickpoll/quickpoll.conf"
#define POLL_FREQ 100

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(tick()));



    QSettings settings(SETTINGS_FILENAME, QSettings::IniFormat);
//    switch(settings.status())
//    {
//    case QSettings::AccessError:
//        _debug(QString("Settings: can't access ") + SETTINGS_FILENAME);
//        break;
//    case QSettings::FormatError:
//        _debug(QString("Settings: format error in ") + SETTINGS_FILENAME);
//        break;
//    default:
//        _debug(QString("Settings: loading ") + SETTINGS_FILENAME);
//    }

    this->indexPath=settings.value("QuickPoll/index-php-path").toString();
    this->votesPath=settings.value("QuickPoll/vote-folder-path").toString();
    this->voteFileDir = new QDir(this->votesPath);
//    if (!this->voteFileDir->exists()) qDebug()<<"error";


    //precarica php
    QFile p(":/php/vote");
    p.open(QIODevice::ReadOnly);
    this->phpVote= p.readAll();
    p.close();

    QFile q(":/php/novote");
    q.open(QIODevice::ReadOnly);
    this->phpNovote= q.readAll();
    q.close();



    this->archive=new QStandardItemModel(0,5,this);
    ui->tableArchive->setModel(this->archive);
    this->arch_itemdelegate = new archiveDelegate(this);
    ui->tableArchive->setItemDelegate(this->arch_itemdelegate);
    cleartable();

    connect(ui->btnSave,SIGNAL(clicked()),this,SLOT(saveFile()));

}

MainWindow::~MainWindow()
{
    delete ui;



}

void MainWindow::tick()
{
    if (ui->checkMaxTimeEnable->isChecked()) {
        //conto alla rovescia
        count--;
        if (count==0) on_btnStop_clicked();
        ui->editMaxTimeSeconds->setValue(count/10);
    }

    //poll
    int conta=0, punti=0;
    this->voteFileDir->refresh();
    QStringList votes = this->voteFileDir->entryList(QStringList() << "*.vot" , QDir::Files | QDir::Readable);
    for(int i = 0; i < votes.count(); i++) {
        QFile file(this->votesPath + "/"+ votes.at(i));
        file.open(QIODevice::ReadOnly);
        QString text=file.readAll();
        punti+=text.toInt();
        conta++;
    }
    ui->lblTotalPolls->setText(QString("%1").arg(conta));
    ui->lblTotalPoints->setText(QString("%1").arg(punti));
}

void MainWindow::on_btnGo_clicked()
{
    count=ui->editMaxTimeSeconds->value()*(1000/POLL_FREQ);
    ui->checkMaxTimeEnable->setEnabled(false);
    ui->btnGo->setEnabled(false);
    ui->btnStop->setEnabled(true);
    ui->btnDel->setEnabled(false);
    ui->btnClear->setEnabled(false);
    ui->btnSave->setEnabled(false);
    ui->editMaxTimeSeconds->setEnabled(false);
    mt=ui->editMaxTimeSeconds->value();

    //timer start
    timer->start(POLL_FREQ);
    this->start_timestamp= QDateTime::currentDateTime().toString();

    //enable voting (voting page)
    createfile(true);
}

void MainWindow::on_btnStop_clicked()
{
    ui->checkMaxTimeEnable->setEnabled(true);
    ui->btnGo->setEnabled(true);
    ui->btnStop->setEnabled(false);
    ui->btnDel->setEnabled(true);
    ui->btnClear->setEnabled(true);
    ui->btnSave->setEnabled(true);
    ui->editMaxTimeSeconds->setEnabled(true);
    ui->editMaxTimeSeconds->setValue(this->mt);

    //stop timer
    timer->stop();

    //disable voting (courtesy page)
    createfile(false);

    //salva il polling
    this->archive->appendRow(QList<QStandardItem*>() << new QStandardItem(ui->pollName->text())<< new QStandardItem(ui->lblTotalPolls->text())
                             <<new QStandardItem(ui->lblTotalPoints->text())<<new QStandardItem(this->start_timestamp)
                             <<new QStandardItem(QDateTime::currentDateTime().toString()) );

    //azzera il polling
    ui->pollName->setText("");
    //remove all polling files (!)


}

void MainWindow::on_pollName_textChanged(const QString &arg1)
{
    if (!arg1.isEmpty()) {
        ui->btnGo->setEnabled(true);
    }else{
        ui->btnGo->setEnabled(false);
    }
}

void MainWindow::createfile(bool enablePoll)
{
    //QFile::remove(this->serverPath+"/index.php");
    QFile file(this->indexPath);
    qDebug()<<"fopen"<<file.open(QIODevice::ReadWrite|QIODevice::Truncate);
    if (enablePoll) {

        file.write(this->phpVote);
    }else{

        file.write(this->phpNovote);
    }
    file.close();
}

void MainWindow::cleartable()
{
    this->archive->clear();
    this->archive->setHorizontalHeaderLabels(QStringList()<<"nome"<<"votanti"<<"punti"<<"timestamp inizio votazioni"<<"timestamp fine votazioni");
    ui->tableArchive->setColumnWidth(0,150);
    ui->tableArchive->setColumnWidth(1,90);
    ui->tableArchive->setColumnWidth(2,90);
    ui->tableArchive->setColumnWidth(3,150);
    ui->tableArchive->setColumnWidth(4,150);
}

void MainWindow::on_tableArchive_doubleClicked(const QModelIndex &index)
{
//    //edit name
//    if (index.column()==0)
//        ui->tableArchive->edit(index);
}


bool MainWindow::saveFile()
{
    if (archive->rowCount()==0) return true;
    QString fn = QFileDialog::getSaveFileName(this, "Salva come...",QString(), "ODS spreadsheet (*.ods);;CSV (*.csv);;Tabbed text (*.txt)");
    if (fn.isEmpty()) return false;
    //salva

    QMessageBox::information(this,"QuickPoll",QString("File %1 salvato correttamente").arg(fn));
    return true;
}

void MainWindow::on_btnClear_clicked()
{
    if (this->archive->rowCount()==0) return;
    int ret=(QMessageBox::warning(this,"QuickPoll",QString("Salvare prima di pulire l'archivio?"),QMessageBox::Save | QMessageBox::Discard
                             | QMessageBox::Cancel, QMessageBox::Cancel));
    if (ret==QMessageBox::Save)
        if (saveFile()) this->cleartable();
    if (ret==QMessageBox::Discard) this->cleartable();
}

void MainWindow::on_btnDel_clicked()
{
    if (ui->tableArchive->selectionModel()->selectedIndexes().isEmpty()) return;
    QModelIndex index= ui->tableArchive->selectionModel()->selectedIndexes().first();
    if (index.isValid())
        if (QMessageBox::warning(this,"QuickPoll",QString("Cancellare la riga %1 ?")
                                 .arg(this->archive->data(this->archive->index(index.row(),0)).toString()),
                                 QMessageBox::No|QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
            this->archive->removeRow(index.row());

}


