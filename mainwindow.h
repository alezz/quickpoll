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
    QString serverPath, votesPath;
    QDir *voteFileDir;

    void createfile(bool enablePoll);

    QString start_timestamp;

    QStandardItemModel * archive;

    QByteArray phpVote, phpNovote;

    archiveDelegate * arch_itemdelegate;
};

#endif // MAINWINDOW_H
