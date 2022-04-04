/* GTlab - Gas Turbine laboratory
 * Source File: gt_processoverviewmodel.cpp
 * copyright 2009-2017 by DLR
 *
 *  Created on: 03.11.2017
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include <QColor>
#include <QIcon>
#include <QDebug>

#include "gt_application.h"
#include "gt_processcategoryitem.h"
#include "gt_icons.h"

#include "gt_processoverviewmodel.h"

GtProcessOverviewModel::GtProcessOverviewModel(QObject* parent) :
    QAbstractItemModel(parent)
{
}

GtProcessOverviewModel::~GtProcessOverviewModel()
{
    qDeleteAll(m_categories);
    m_categories.clear();
}

int
GtProcessOverviewModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
    {
        return m_categories.size();
    }

    GtAbstractProcessItem* parentItem = itemFromIndex(parent);

    if (!parentItem)
    {
        return 0;
    }

    return parentItem->findDirectChildren<GtAbstractProcessItem*>().count();
}

int
GtProcessOverviewModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 2;
}

QVariant
GtProcessOverviewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    const int col = index.column();
    const int row = index.row();

    if (row < 0)
    {
        return QVariant();
    }

    GtAbstractProcessItem* abstractItem = itemFromIndex(index);

    if (!abstractItem)
    {
        return QVariant();
    }

    if (qobject_cast<GtProcessCategoryItem*>(abstractItem))
    {
        GtProcessCategoryItem* catItem =
            qobject_cast<GtProcessCategoryItem*>(abstractItem);

        switch (role)
        {
            case Qt::DisplayRole:
                if (col == 0)
                {
                    return abstractItem->objectName();
                }

                break;


            case Qt::DecorationRole:
                if (col == 0)
                {
                    if (catItem->collapsed())
                    {
                        return GtGUI::icon(
                                    QStringLiteral("arrowrightIcon.png"));
                    }

                    return GtGUI::icon(QStringLiteral("arrowdownIcon.png"));
                }

                break;

            case Qt::BackgroundRole:
                if (!gtApp->inDarkMode())
                {
                    return QColor(246, 246, 246);
                }
                break;
            case CategoryRole:
                return true;

            default:
                break;
        }
    }
    else
    {
        switch (role)
        {
            case Qt::DisplayRole:
                if (col == 0)
                {
                    return id(abstractItem);
                }
                else if (col == 1)
                {
                    return version(abstractItem);
                }

                break;

            case Qt::DecorationRole:
                if (col == 0)
                {
                    return icon(abstractItem);
                }

                break;

            case Qt::ForegroundRole:
                if (col == 1)
                {
                    if (!gtApp->inDarkMode())
                    {
                        return QColor(Qt::gray);
                    }
                }

                break;

            case Qt::ToolTipRole:
                return description(abstractItem);

        }
    }

    if (role == CategoryRole)
    {
        return false;
    }

    return QVariant();
}

bool
GtProcessOverviewModel::setData(const QModelIndex& index,
                                const QVariant& value, int role)
{
    if (role == CollapseRole)
    {
        GtAbstractProcessItem* pItem = itemFromIndex(index);

        if (!pItem)
        {
            return false;
        }

        GtProcessCategoryItem* catItem =
            qobject_cast<GtProcessCategoryItem*>(pItem);

        if (!catItem)
        {
            return false;
        }

        catItem->setCollapsed(value.toBool());
        emit dataChanged(index, index);
    }

    return QAbstractItemModel::setData(index, value, role);
}

QModelIndex
GtProcessOverviewModel::index(int row, int col,
                              const QModelIndex& parent) const
{
    if (!parent.isValid())
    {
        const int catCount = m_categories.count();

        if (row < 0 || row >= catCount)
        {
            return {};
        }

        return createIndex(row, col, m_categories[row]);
    }

    GtAbstractProcessItem* parentItem = itemFromIndex(parent);

    if (!parentItem)
    {
        return {};
    }

    GtAbstractProcessItem* childItem =
        parentItem->findDirectChildren<GtAbstractProcessItem*>().value(row);

    if (!childItem)
    {
        return {};
    }

    return createIndex(row, col, childItem);
}

QModelIndex
GtProcessOverviewModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return {};
    }

    GtAbstractProcessItem* childItem = itemFromIndex(index);

    if (!childItem)
    {
        return {};
    }

    GtObject* parentItem = childItem->parentObject();

    if (!parentItem)
    {
        return {};
    }

    return indexFromItem(qobject_cast<GtAbstractProcessItem*>(parentItem));
}

GtAbstractProcessItem*
GtProcessOverviewModel::itemFromIndex(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return nullptr;
    }

    if (index.model() != this)
    {
        return nullptr;
    }

    return static_cast<GtAbstractProcessItem*>(index.internalPointer());
}

GtProcessCategoryItem*
GtProcessOverviewModel::categoryItem(const QString& id)
{
    foreach (GtProcessCategoryItem* i, m_categories)
    {
        if (i->objectName() == id)
        { // cppcheck-suppress useStlAlgorithm
            return i;
        }
    }

    GtProcessCategoryItem* newItem = new GtProcessCategoryItem(id, this);

    m_categories << newItem;

    return newItem;
}

QModelIndex
GtProcessOverviewModel::indexFromItem(GtAbstractProcessItem* item) const
{
    if (!item)
    {
        return {};
    }

    int row = -1;

    if (!item->parent())
    {
        row = m_categories.indexOf(qobject_cast<GtProcessCategoryItem*>(item));
    }
    else
    {
        if (item->parent())
        {
            row = item->childNumber();
        }
        else
        {
            qWarning() << "WARNING (GtCalculatorOverviewModel::indexFromObject): " <<
                       "object has no parent!";
            qWarning() << " |-> obj = " << item->objectName();
        }
    }

    if (row == -1)
    {
        qWarning() << "WARNING (GtCalculatorOverviewModel::indexFromObject): "
                   << "row == -1!";
        return {};
    }

    return createIndex(row, 0, item);
}
