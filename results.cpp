#include "results.h"
#include "ui_results.h"
#include "archivedelegate.h"

Results::Results(QAbstractItemModel * model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Results)
{
    ui->setupUi(this);

    archiveDelegate * del = new archiveDelegate(this);
    ui->tableView->setItemDelegate(del);

    ui->tableView->setModel(model);

}

Results::~Results()
{
    delete ui;
}

void Results::hideEvent(QHideEvent *)
{
    emit hidden(false);
}

void Results::resettable()
{
    ui->tableView ->setColumnWidth(0,150);
    ui->tableView->setColumnWidth(1,90);
    ui->tableView->setColumnWidth(2,90);
    ui->tableView->setColumnWidth(3,90);
    ui->tableView->setColumnWidth(4,0);
    ui->tableView->setColumnWidth(5,0);
}
