//This file is part of Quickpoll.

//Author: Mauro Alessandro (www.maetech.it) (c) march 2014

//Quickpoll is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//QuickPoll is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with QuickPoll.  If not, see <http://www.gnu.org/licenses/>.


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QStandardItemModel>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <cmath>
#include <QTextStream>

#define SETTINGS_FILENAME "quickpoll.conf"
#define POLL_FREQ 500
#define DATETIME_FORMAT "dd/MM/yy hh:mm:ss"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

bool MainWindow::setup()
{
    this->timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(tick()));


    this->archive=new QStandardItemModel(0,6,this);
    ui->tableArchive->setModel(this->archive);
    this->arch_itemdelegate = new archiveDelegate(this);
    ui->tableArchive->setItemDelegate(this->arch_itemdelegate);

    if (!QFile::exists(SETTINGS_FILENAME)) {
        QMessageBox::critical(this,"QuickPoll", "Errore grave: non trovo il file di configurazione");
        //eventualmente implementare qui la creazione di un file standard
        return false;
    }
    QSettings settings(SETTINGS_FILENAME, QSettings::IniFormat);
    if (settings.status()==QSettings::FormatError) {
        QMessageBox::critical(this,"QuickPoll", "Errore grave: ci sono errori nel file di configurazione");
        return false;
    }

    this->indexPath=settings.value("QuickPoll/index-php-path").toString();

    this->votesPath=settings.value("QuickPoll/vote-folder-path").toString();
    this->voteFileDir = new QDir(this->votesPath);
    if (!this->voteFileDir->exists()) {
        QMessageBox::critical(this,"QuickPoll", "Errore grave: non esiste la directory per i file dei voti specificata nel file di configurazione");
        return false;
    }

    connect(ui->btnSave,SIGNAL(clicked()),this,SLOT(saveFile()));

    this->dResults=new Results(this->archive,this);
    QFile dhf( settings.value("QuickPoll/display-html-path").toString() );
    if (dhf.open(QIODevice::ReadOnly)) this->dSign=new Dialog(dhf.readAll(), this);
    else {
        this->dSign=new Dialog("%polling%",this);
        QMessageBox::warning(this,"QuickPoll","Non riesco ad aprire un file. La casella opzionale di stato votazioni non si vedra' correttamente");
    }
    dhf.close();

    this->dAbout=new About(this);

    connect(ui->btnShow,SIGNAL(clicked(bool)),this->dResults,SLOT(setVisible(bool)));
    connect(ui->btnSign,SIGNAL(clicked(bool)),this->dSign,SLOT(setVisible(bool)));
    connect(this->dResults,SIGNAL(hidden(bool)),ui->btnShow,SLOT(setChecked(bool)));
    connect(this->dSign,SIGNAL(hidden(bool)),ui->btnSign,SLOT(setChecked(bool)));
    connect(ui->btnAbout,SIGNAL(clicked()),this->dAbout,SLOT(show()));


    //precarica php TODO throw errors
    QString path = settings.value("QuickPoll/vote-php-path").toString();
    QFile p(path);
    if (p.open(QIODevice::ReadOnly)) {
        this->phpVote= p.readAll();
        p.close();
    }else{
        QMessageBox::critical(this,"QuickPoll","Non riesco ad aprire il file php per la votazione specificato nel file di configurazione - Impossibile continuare");
        return false;
    }
    path=settings.value("QuickPoll/novote-php-path").toString();
    QFile q(path);
    if (q.open(QIODevice::ReadOnly)) {
        this->phpNovote= q.readAll();
        q.close();
    }else{
        QMessageBox::critical(this,"QuickPoll","Non riesco ad aprire il file php per la non-votazione specificato nel file di configurazione - Impossibile continuare");
        return false;
    }


    //inizializza l'index.php nel webserver
    if (!createfile(false)) {
        QMessageBox::critical(this,"QuickPoll","Non riesco a scrivere il file index.php nel percorso specificato nel file di configurazione - Impossibile continuare");
        return false;
    }


    //inizializza la tabella archivio votazioni
    cleartable();


    return true;
}

MainWindow::~MainWindow()
{
    delete ui;

    //cancella le dialog
    delete this->dSign;
    delete this->dAbout;
    delete this->dResults;

    //cancella tutti i file
    QFile::remove(this->indexPath);
    clearall();

}

void MainWindow::tick()
{
    if (ui->checkMaxTimeEnable->isChecked()) {
        //conto alla rovescia
        count--;
        ui->editMaxTimeSeconds->setValue((count*POLL_FREQ)/1000);
        if (count==0) on_btnStop_clicked();
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
    if (conta>0)
        ui->lblAveragePoints->setText(QString("%1").arg(round((float)punti/conta)));
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
    this->start_timestamp= QDateTime::currentDateTime().toString(DATETIME_FORMAT);

    //enable voting (voting page)
    createfile(true);

    //display
    this->dSign->display(ui->pollName->text(),true,false);
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

    //stop timer
    timer->stop();
    ui->editMaxTimeSeconds->setValue(this->mt);


    //disable voting (courtesy page)
    createfile(false);

    //salva il polling
    this->archive->appendRow(QList<QStandardItem*>() << new QStandardItem(ui->pollName->text())<< new QStandardItem(ui->lblTotalPolls->text())
                             <<new QStandardItem(ui->lblTotalPoints->text())<<new QStandardItem(ui->lblAveragePoints->text())
                             <<new QStandardItem(this->start_timestamp)<<new QStandardItem(QDateTime::currentDateTime().toString(DATETIME_FORMAT)) );

    //azzera il polling
    ui->pollName->setText("");

    //remove all polling files
    clearall();

    //display
    this->dSign->display(ui->pollName->text(),false,true);
}

void MainWindow::on_pollName_textChanged(const QString &arg1)
{
    if (!arg1.isEmpty()) {
        ui->btnGo->setEnabled(true);
        //display
        this->dSign->display(arg1,false,false);
    }else{
        ui->btnGo->setEnabled(false);
    }
}

bool MainWindow::createfile(bool enablePoll)
{
    QFile file(this->indexPath);
    if (file.open(QIODevice::ReadWrite|QIODevice::Truncate)) {
        if (enablePoll) {
            QByteArray tmp(this->phpVote);
            QByteArray n( ui->pollName->text().toLocal8Bit());
            tmp.replace("%pollname%",n);
            file.write(tmp);
        }else{
            file.write(this->phpNovote);
        }
        file.close();
        return true;
    }else{
        return false;
    }
}

void MainWindow::cleartable()
{
    this->archive->clear();
    this->archive->setHorizontalHeaderLabels(QStringList()<<"nome"<<"votanti"<<"punti tot"<<"media pt."<<"ora inizio votazioni"<<"ora fine votazioni");
    ui->tableArchive->setColumnWidth(0,150);
    ui->tableArchive->setColumnWidth(1,90);
    ui->tableArchive->setColumnWidth(2,90);
    ui->tableArchive->setColumnWidth(3,90);
    ui->tableArchive->setColumnWidth(4,150);
    ui->tableArchive->setColumnWidth(5,150);
    this->dResults->resettable();
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
//    QFileDialog fileDlg(this,"Salva come...",QString(), "CSV (*.csv);;Tabbed text (*.txt)");
//    fileDlg.selectNameFilter("CSV (*.csv)");
//    fileDlg.exec();
//    QString fn = fileDlg.fileSelected();
    QString fn=QFileDialog::getSaveFileName(this, "Salva come...",QString(), "CSV (*.csv)");
    if (fn.isEmpty()) return false;
    if (!fn.endsWith(".csv")) fn.append(".csv");
    //salva
    QFile f(fn);
//    if (f.exists()) {
//        int ret=QMessageBox::question(this,"QuickPoll","Il file esiste, lo sovrascrivo?",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
//        if (ret==   QMessageBox::No) return false;
//    }
    if (f.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text)) {
        QTextStream output(&f);
        for (int i=0; i<this->archive->rowCount();i++)
        {
            for (int j=0; j<5;j++)
            {
                if (j==0) output<<'"';
                output<<archive->data(archive->index(i,j)).toString();
                if (j==0) output<<'"';
                if (j<4) output<<",";
            }
            output<<"\n";
        }
        f.close();
        QMessageBox::information(this,"QuickPoll",QString("File %1 salvato correttamente").arg(fn));
        return true;
    } else {
        QMessageBox::warning(this,"QuickPoll","File non salvato. C'è stato un errore nell'aprire il file scelto, provare con un altro percorso");
        return false;
    }

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if (archive->rowCount()>0) {
        int res=QMessageBox::warning(this,"QuickPoll","Abbandonare la lista corrente e uscire??",QMessageBox::Save|QMessageBox::Discard|
                                     QMessageBox::Cancel,QMessageBox::Cancel);
        if ((res==QMessageBox::Save)&&(saveFile())) event->accept();
        if (res==QMessageBox::Discard)     event->accept();
    } else {
        int res=QMessageBox::question(this,"QuickPoll","Sicuro di voler uscire??",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
        if (res==QMessageBox::Yes) event->accept();
    }
}

void MainWindow::clearall()
{
    //cancella tutti i file di voto
    //

    QStringList votes = this->voteFileDir->entryList(QStringList() << "*.vot" , QDir::Files | QDir::Readable);
    for(int i = 0; i < votes.count(); i++)
        QFile::remove(this->votesPath + "/"+ votes.at(i));

}
