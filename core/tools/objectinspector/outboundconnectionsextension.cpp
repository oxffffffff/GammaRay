/*
  outboundconnectionsextension.cpp

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2014 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Anton Kreuzkamp <anton.kreuzkamp@kdab.com>

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

#include "outboundconnectionsextension.h"
#include "connectionfilterproxymodel.h"
#include "propertycontroller.h"
#include "probe.h"
#include <QMetaProperty>

using namespace GammaRay;

OutboundConnectionsExtension::OutboundConnectionsExtension(PropertyController* controller) :
  PropertyControllerExtension(controller->objectBaseName() + ".outboundConnections"),
  m_model(new ConnectionFilterProxyModel(controller))
{
  m_model->setFilterOnSender(true);
  controller->registerModel(m_model, "outboundConnections");

  // TODO 1.3 had an optimization using the ProxyDetacher for these models, re-add that
  m_model->setSourceModel(Probe::instance()->connectionModel());
}

OutboundConnectionsExtension::~OutboundConnectionsExtension()
{
}

bool OutboundConnectionsExtension::setQObject(QObject* object)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  m_model->filterSender(object);
  return true;
#else
  Q_UNUSED(object);
  return false;
#endif
}
