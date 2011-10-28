/*
  statemodel.cpp

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2010-2011 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
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
#include "statemodel.h"

#include "util.h"
#include "statemachinewatcher.h"

#include <QtCore/QStringList>
#include <QtCore/QStateMachine>
#include <QtCore/QAbstractTransition>
#include <QtCore/QDebug>

// #include <modeltest.h>

using namespace GammaRay;

namespace GammaRay
{

class StateModelPrivate
{
  StateModelPrivate(StateModel *qq)
    : q_ptr(qq)
    , m_stateMachine(0)
    , m_stateMachineWatcher(new StateMachineWatcher(qq))
  {
    Q_ASSERT(qq->connect(m_stateMachineWatcher, SIGNAL(transitionTriggered(QAbstractTransition*)), qq, SLOT(transitionTriggered(QAbstractTransition*))));
  }

  Q_DECLARE_PUBLIC(StateModel)
  StateModel * const q_ptr;
  StateMachineWatcher * const m_stateMachineWatcher;
  QStateMachine *m_stateMachine;

  QList<QObject*> children(QObject *parent) const;

  QObject *mapModelIndex2QObject(const QModelIndex &) const;

// private slots:
  void transitionTriggered(QAbstractTransition*);
};

}

QList<QObject*> StateModelPrivate::children(QObject *parent) const
{
  QList<QObject*> result;
  if (parent == 0) {
    parent = m_stateMachine;
  }

  // if the state machine is not yet set, return an empty list
  if (!parent)
    return result;

  foreach (QObject *o, parent->children()) {
    if (o->inherits("QState")) {
      result.append(o);
    }
  }

  qSort(result);
  return result;
}

QObject *StateModelPrivate::mapModelIndex2QObject(const QModelIndex &index) const
{
  if (index.isValid()) {
    QObjectList c = children(reinterpret_cast<QObject*>(index.internalPointer()));
    return c[index.row()];
  }
  return m_stateMachine;
}

void StateModelPrivate::transitionTriggered(QAbstractTransition* transition)
{
  Q_Q(StateModel);

  // TODO: Make this more efficient? Find out the changed states and update just these indices
  q->dataChanged(QModelIndex(), QModelIndex());
}

StateModel::StateModel(QObject *parent)
  : ObjectModelBase<QAbstractItemModel>(parent), d_ptr(new StateModelPrivate(this))
{
  QHash<int, QByteArray> _roleNames = roleNames();
  _roleNames.insert(TransitionsRole, "transitions");
  _roleNames.insert(IsInitialStateRole, "isInitial");
  setRoleNames(_roleNames);
}

void StateModel::setStateMachine(QStateMachine* stateMachine)
{
  Q_D(StateModel);
  if (d->m_stateMachine == stateMachine)
    return;

  beginResetModel();
  d->m_stateMachine = stateMachine;
  endResetModel();

  d->m_stateMachineWatcher->setWatchedStateMachine(stateMachine);
}

QStateMachine *StateModel::stateMachine() const
{
  Q_D(const StateModel);
  return d->m_stateMachine;
}

QVariant StateModel::data(const QModelIndex &index, int role) const
{
  Q_D(const StateModel);
  if (!index.isValid()) {
    return QVariant();
  }

  if (role == TransitionsRole) {
    QObject *obj = d->mapModelIndex2QObject(index);
    QState *state = qobject_cast<QState*>(obj);
    if (state) {
      QObjectList l = d->children(state->parent());
      Q_ASSERT(l.contains(state));
      QStringList nums;
      QList<QAbstractTransition*> trs = state->transitions();
      foreach (QAbstractTransition *t, trs) {
        QAbstractState *child = t->targetState();
        Q_ASSERT(l.contains(child));
        nums << QString::number(l.indexOf(child) - l.indexOf(state));
      }
      return nums.join(",");
    }
  }
  if (role == IsInitialStateRole) {
    QObject *obj = d->mapModelIndex2QObject(index);
    QState *state = qobject_cast<QState*>(obj);
    if (state) {
      QState *parentState = state->parentState();
      return (state == parentState->initialState());
    }
  }

  QObject *obj = d->mapModelIndex2QObject(index);
  if (obj) {
    if (role == StateObjectRole) {
      return QVariant::fromValue(obj);
    }

    if (index.column() == 0 && role == Qt::CheckStateRole) {
      QState *s = qobject_cast<QState*>(obj);
      if (s) {
        return d->m_stateMachine->configuration().contains(s);
      }
    }

    return dataForObject(obj, index, role);
  }
  return QVariant();
}

int StateModel::rowCount(const QModelIndex &parent) const
{
  Q_D(const StateModel);
  return d->children(d->mapModelIndex2QObject(parent)).count();
}

QModelIndex StateModel::index(int row, int column, const QModelIndex &parent) const
{
  Q_D(const StateModel);
  if (row < 0 || column < 0 || column > 1) {
    return QModelIndex();
  }

  QObject *internalPointer = reinterpret_cast<QObject*>(parent.internalPointer());
  if (!parent.isValid()) {
    internalPointer = d->m_stateMachine;
  } else {
    QObject *o = reinterpret_cast<QObject*>(parent.internalPointer());
    QObjectList c = d->children(o);
    internalPointer = c.at(parent.row());
  }

  QObjectList c = d->children(internalPointer);
  if (row >= c.size()) {
    return QModelIndex();
  }

  return createIndex(row, column, internalPointer);
}

QModelIndex StateModel::parent(const QModelIndex &index) const
{
  Q_D(const StateModel);
  if (!index.isValid()) {
    return QModelIndex();
  }
  QObject *obj = d->mapModelIndex2QObject(index);
  QObject *parent = obj->parent();

  if (parent == d->m_stateMachine) {
    return QModelIndex();
  }

  QObject *grandParent = parent->parent();
  int row = d->children(grandParent).indexOf(parent);
  return createIndex(row, 0, grandParent);
}

#include "statemodel.moc"