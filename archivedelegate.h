#ifndef ARCHIVEDELEGATE_H
#define ARCHIVEDELEGATE_H

#include <QItemDelegate>
#include <QPainter>

class archiveDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit archiveDelegate(QObject *parent = 0);

    QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    void	paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

signals:

public slots:

};

#endif // ARCHIVEDELEGATE_H
