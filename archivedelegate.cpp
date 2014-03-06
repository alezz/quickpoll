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
    QRect r=option.rect;
    if (index.column()==1) painter->fillRect(r,QColor(180,255,255));
    if (index.column()==2) painter->fillRect(r,QColor(180,255,180));
    if (index.column()==3) painter->fillRect(r,QColor(255,255,180));
    if (option.state & QStyle::State_Selected)
        painter->fillRect(r, QColor(128,180,255));
    painter->drawText(r,index.model()->data(index).toString(),QTextOption(Qt::AlignCenter));


}
