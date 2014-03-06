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



#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QStandardItemModel>
#include "archivedelegate.h"
#include "dialog.h"
#include "results.h"
#include "about.h"
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool setup();
    ~MainWindow();



private slots:
    void tick();

    void on_btnGo_clicked();
    void on_btnStop_clicked();

    void on_pollName_textChanged(const QString &arg1);

    void cleartable();

    void on_tableArchive_doubleClicked(const QModelIndex &index);



    void on_btnClear_clicked();

    void on_btnDel_clicked();

    bool saveFile();

    void closeEvent(QCloseEvent *event);

    void clearall();

    void clearcounters();

    void poll();

private:


    Ui::MainWindow *ui;
    QTimer *timer;
    int count,mt;
    QString indexPath, votesPath;
    QDir *voteFileDir;

    bool createfile(bool enablePoll);

    QString start_timestamp;

    QStandardItemModel * archive;

    QByteArray phpVote, phpNovote;

    archiveDelegate * arch_itemdelegate;

    Results * dResults;
    Dialog * dSign;
    About * dAbout;
};

#endif // MAINWINDOW_H
