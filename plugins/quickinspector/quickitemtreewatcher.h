/*
  quickitemtreewatcher.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2014 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Volker Krause <volker.krause@kdab.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GAMMARAY_QUICKITEMTREEWATCHER_H
#define GAMMARAY_QUICKITEMTREEWATCHER_H

#include <QObject>

class QModelIndex;
class QTreeView;

namespace GammaRay {

/** Auto-expand the visible item sub-tree. */
class QuickItemTreeWatcher : public QObject
{
    Q_OBJECT
public:
    explicit QuickItemTreeWatcher(QTreeView* view, QTreeView *sgView, QObject *parent = 0);
    ~QuickItemTreeWatcher();

private slots:
    void itemModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void sgModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    QTreeView *m_itemView;
    QTreeView *m_sgView;
};

}

#endif // GAMMARAY_QUICKITEMTREEWATCHER_H
