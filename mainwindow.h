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



#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QStandardItemModel>
#include "archivedelegate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
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

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int count,mt;
    QString indexPath, votesPath;
    QDir *voteFileDir;

    void createfile(bool enablePoll);

    QString start_timestamp;

    QStandardItemModel * archive;

    QByteArray phpVote, phpNovote;

    archiveDelegate * arch_itemdelegate;
};

#endif // MAINWINDOW_H
