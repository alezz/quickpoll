#include "archivedelegate.h"

archiveDelegate::archiveDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}


QSize	archiveDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QSize( option.rect.width(),option.rect.height());
}

void archiveDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{

    if (index.column()==2) painter->fillRect(option.rect,QColor(255,255,180));
    painter->drawText(option.rect,index.model()->data(index).toString(),QTextOption(Qt::AlignCenter));

}
