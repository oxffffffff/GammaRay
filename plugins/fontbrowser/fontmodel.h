/*
  fontmodel.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2010-2014 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Stephen Kelly <stephen.kelly@kdab.com>

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

#ifndef GAMMARAY_FONTBROWSER_FONTMODEL_H
#define GAMMARAY_FONTBROWSER_FONTMODEL_H

#include <QAbstractItemModel>
#include <QFont>
#include <QColor>

namespace GammaRay {

class FontModel : public QAbstractItemModel
{
  Q_OBJECT
  public:
    explicit FontModel(QObject *parent);

    void updateFonts(const QList<QFont> &fonts);
    QList<QFont> currentFonts() const;

    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const;

    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;

    virtual QModelIndex parent(const QModelIndex &child) const;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

  public slots:
    void updateText(const QString &text);
    void toggleBoldFont(bool bold);
    void toggleItalicFont(bool italic);
    void toggleUnderlineFont(bool underline);
    void setPointSize(int size);
    void setColors(const QColor &foreground, const QColor &background);

  private:
    void fontDataChanged();

    QList<QFont> m_fonts;
    QString m_text;
    int m_size;
    bool m_bold;
    bool m_italic;
    bool m_underline;
    QColor m_foreground;
    QColor m_background;
};

}

#endif
