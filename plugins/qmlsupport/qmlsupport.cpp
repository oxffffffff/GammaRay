/*
  qmlsupport.cpp

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

#include "qmlsupport.h"

#include <core/metaobject.h>
#include <core/metaobjectrepository.h>
#include <core/varianthandler.h>

#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlError>

Q_DECLARE_METATYPE(QQmlError)

using namespace GammaRay;

static QString qmlErrorToString(const QQmlError &error)
{
  return QString::fromLatin1("%1:%2:%3: %4")
    .arg(error.url().toString())
    .arg(error.line())
    .arg(error.column())
    .arg(error.description());
}

QmlSupport::QmlSupport(GammaRay::ProbeInterface* probe, QObject* parent) :
  QObject(parent)
{
  Q_UNUSED(probe);

  MetaObject *mo = 0;
  MO_ADD_METAOBJECT1(QQmlComponent, QObject);
  MO_ADD_PROPERTY_RO(QQmlComponent, QList<QQmlError>, errors);
  MO_ADD_PROPERTY_RO(QQmlComponent, bool, isError);
  MO_ADD_PROPERTY_RO(QQmlComponent, bool, isLoading);
  MO_ADD_PROPERTY_RO(QQmlComponent, bool, isNull);
  MO_ADD_PROPERTY_RO(QQmlComponent, bool, isReady);

  MO_ADD_METAOBJECT1(QQmlContext, QObject);
  MO_ADD_PROPERTY_CR(QQmlContext, QUrl, baseUrl, setBaseUrl);
  MO_ADD_PROPERTY   (QQmlContext, QObject*, contextObject, setContextObject);
  MO_ADD_PROPERTY_RO(QQmlContext, QQmlEngine*, engine);
  MO_ADD_PROPERTY_RO(QQmlContext, bool, isValid);
  MO_ADD_PROPERTY_RO(QQmlContext, QQmlContext*, parentContext);

  MO_ADD_METAOBJECT1(QQmlEngine, QObject);
  MO_ADD_PROPERTY_CR(QQmlEngine, QUrl, baseUrl, setBaseUrl);
  MO_ADD_PROPERTY_CR(QQmlEngine, QStringList, importPathList, setImportPathList);
  MO_ADD_PROPERTY   (QQmlEngine, bool, outputWarningsToStandardError, setOutputWarningsToStandardError);
  MO_ADD_PROPERTY_CR(QQmlEngine, QStringList, pluginPathList, setPluginPathList);
  MO_ADD_PROPERTY_RO(QQmlEngine, QQmlContext*, rootContext);

  VariantHandler::registerStringConverter<QQmlError>(qmlErrorToString);
}

QString QmlSupportFactory::name() const
{
  return tr("QML Support");
}
