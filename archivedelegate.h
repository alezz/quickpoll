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
