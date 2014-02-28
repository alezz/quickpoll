#ifndef RESULTS_H
#define RESULTS_H

#include <QDialog>
#include <QAbstractItemModel>

namespace Ui {
class Results;
}

class Results : public QDialog
{
    Q_OBJECT

public:
    explicit Results( QAbstractItemModel * model, QWidget *parent = 0);
    ~Results();
    void resettable();

public slots:
    void hideEvent(QHideEvent *);

signals:
    void hidden(bool);

private:
    Ui::Results *ui;
};

#endif // RESULTS_H
