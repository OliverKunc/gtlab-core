/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 24.07.2015
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include <QUuid>
#include <QSignalMapper>
#include <QDebug>

#include "gt_objectmemento.h"
#include "gt_abstractobjectfactory.h"
#include "gt_abstractproperty.h"
#include "gt_result.h"
#include "gt_logging.h"
#include "gt_label.h"
#include "gt_objectmementodiff.h"
#include "gt_objectio.h"
#include "gt_dummyobject.h"

#include "gt_object.h"

GtObject::GtObject(GtObject* parent) :
    m_factory(NULL),
    m_propertyMapper(new QSignalMapper(this))
{
    if (parent)
    {
        parent->appendChild(this);
    }

    // initialize uuid
    newUuid();

    // set newly created flag
    setFlag(GtObject::NewlyCreated);

    connect(m_propertyMapper, SIGNAL(mapped(QObject*)),
            SLOT(propertyChanged(QObject*)));
    connect(this, SIGNAL(objectNameChanged(QString)), SLOT(changed()));
}

GtObject::ObjectFlags
GtObject::objectFlags()
{
    return m_objectFlags;
}

bool
GtObject::isDummy()
{
    if (qobject_cast<GtDummyObject*>(this) != Q_NULLPTR)
    {
        return true;
    }

    return false;
}

bool
GtObject::hasDummyChildren()
{
    foreach (GtObject* c, findChildren<GtObject*>())
    {
        if (c->isDummy())
        {
            return true;
        }
    }

    return false;
}

GtObjectMemento
GtObject::toMemento(bool clone)
{
    return GtObjectMemento(this, clone);
}

void
GtObject::fromMemento(const GtObjectMemento& memento)
{
    // check for factory
    if (m_factory == NULL)
    {
        qDebug() << "factory is null";
        return;
    }

    // merge data
    memento.mergeTo(this, m_factory);
}

bool
GtObject::applyDiff(GtObjectMementoDiff& diff)
{
    return GtObjectIO::applyDiff(diff, this);
}

bool
GtObject::revertDiff(GtObjectMementoDiff& diff)
{
    return GtObjectIO::revertDiff(diff, this);
}

GtObject*
GtObject::copy()
{
    // check for factory
    if (m_factory == NULL)
    {
        gtError() << tr("No factory set!") << QStringLiteral("(")
                  << objectName() << QStringLiteral(")");
        return NULL;
    }

    // generate memento
    GtObjectMemento memento = toMemento(false);

    if (memento.isNull())
    {
        return NULL;
    }

    return memento.restore(m_factory);
}

GtObject*
GtObject::clone()
{
    // check for factory
    if (m_factory == NULL)
    {
        gtError() << tr("No factory set!") << QStringLiteral("(")
                  << objectName() << QStringLiteral(")");
        return NULL;
    }

    // generate memento
    GtObjectMemento memento = toMemento(true);

    if (memento.isNull())
    {
        return NULL;
    }

    return memento.restore(m_factory);
}

bool
GtObject::appendChild(GtObject* c)
{
    if (c == NULL)
    {
        return false;
    }

    if (children().contains(c))
    {
        return false;
    }

    if (!childAccepted(c))
    {
        gtDebug() << tr("child type not accepted!");
        return false;
    }

    // disconnect old signals and slots
    if (c->parent() != Q_NULLPTR)
    {
        disconnect(c, SIGNAL(dataChanged(GtObject*)),
                   c->parent(), SIGNAL(dataChanged(GtObject*)));
        disconnect(c, SIGNAL(dataChanged(GtObject*, GtAbstractProperty*)),
                   c->parent(),
                   SIGNAL(dataChanged(GtObject*, GtAbstractProperty*)));
        disconnect(c, SIGNAL(dataChanged(GtObject*, GtAbstractProperty*)),
                   c->parent(), SLOT(onChildDataChanged()));
        disconnect(c, SIGNAL(dataChanged(GtObject*)),
                   c->parent(), SLOT(onChildDataChanged()));
        disconnect(c, SIGNAL(destroyed(QObject*)),
                   c->parent(), SLOT(changed()));
        disconnect(c, SIGNAL(childAppended(GtObject*, GtObject*)),
                   c->parent(), SIGNAL(childAppended(GtObject*, GtObject*)));
    }

    c->setParent(this);

    connect(c, SIGNAL(dataChanged(GtObject*)), SIGNAL(dataChanged(GtObject*)),
            Qt::DirectConnection);
    connect(c, SIGNAL(childAppended(GtObject*, GtObject*)),
            SIGNAL(childAppended(GtObject*, GtObject*)),
            Qt::DirectConnection);
    connect(c, SIGNAL(dataChanged(GtObject*, GtAbstractProperty*)),
            SIGNAL(dataChanged(GtObject*, GtAbstractProperty*)),
            Qt::DirectConnection);
    connect(c, SIGNAL(dataChanged(GtObject*, GtAbstractProperty*)),
            SLOT(onChildDataChanged()),
            Qt::DirectConnection);
    connect(c, SIGNAL(dataChanged(GtObject*)), SLOT(onChildDataChanged()),
            Qt::DirectConnection);
    connect(c, SIGNAL(destroyed(QObject*)), SLOT(changed()),
            Qt::DirectConnection);

    changed();

    emit childAppended(c, this);

    return true;
}

bool
GtObject::appendChildren(const QList<GtObject*>& list)
{
    foreach (GtObject* c, list)
    {
        if (!appendChild(c))
        {
            return false;
        }
    }

    return true;
}

void
GtObject::disconnectFromParent()
{
    if (parent() != Q_NULLPTR)
    {
        disconnect(this, SIGNAL(dataChanged(GtObject*)),
                   parent(), SIGNAL(dataChanged(GtObject*)));
        disconnect(this, SIGNAL(dataChanged(GtObject*, GtAbstractProperty*)),
                   parent(),
                   SIGNAL(dataChanged(GtObject*, GtAbstractProperty*)));
        disconnect(this, SIGNAL(dataChanged(GtObject*, GtAbstractProperty*)),
                   parent(), SLOT(onChildDataChanged()));
        disconnect(this, SIGNAL(dataChanged(GtObject*)),
                   parent(), SLOT(onChildDataChanged()));
        disconnect(this, SIGNAL(destroyed(QObject*)),
                   parent(), SLOT(changed()));
        disconnect(this, SIGNAL(childAppended(GtObject*, GtObject*)),
                   parent(), SIGNAL(childAppended(GtObject*, GtObject*)));
    }

    setParent(Q_NULLPTR);
}

QString
GtObject::uuid() const
{
    return m_uuid;
}

void
GtObject::setUuid(const QString& val)
{
    if (val.isEmpty())
    {
        return;
    }

    m_uuid = val;
}

void
GtObject::newUuid(bool renewChildUUIDs)
{
    m_uuid = QUuid::createUuid().toString();

    if (renewChildUUIDs)
    {
        newChildUUIDs(this);
    }
}

QString
GtObject::calcHash()
{
    GtObjectMemento mem = this->toMemento();
    mem.calculateHashes();

    return mem.fullHash().toHex();
}

bool
GtObject::isDefault()
{
    return (objectFlags() & GtObject::DefaultComponent);
}

void
GtObject::setDefault(bool val)
{
    setFlag(GtObject::DefaultComponent, val);
}

bool
GtObject::isUserHidden()
{
    return (objectFlags() & GtObject::UserHidden);
}

void
GtObject::setUserHidden(bool val)
{
    setFlag(GtObject::UserHidden, val);
}

bool
GtObject::isRenamable()
{
    return (objectFlags() & GtObject::UserRenamable);
}

bool
GtObject::isDeletable()
{
    if (isDefault())
    {
        return false;
    }

    return (objectFlags() & GtObject::UserDeletable);
}

void
GtObject::setFactory(GtAbstractObjectFactory* factory)
{
    if (factory == NULL && !factory->knownClass(metaObject()->className()))
    {
        return;
    }

    m_factory = factory;
}

int
GtObject::childNumber()
{
    GtObject* p = parentObject();

    if (p)
    {
        return p->findDirectChildren<GtObject*>().indexOf(this);
    }

    return -1;
}

GtObject*
GtObject::parentObject()
{
    return qobject_cast<GtObject*>(parent());
}

GtResult*
GtObject::results()
{
    return findDirectChild<GtResult*>();
}

QStringList
GtObject::labelIds()
{
    QStringList retval;

    foreach (GtLabel* label, findChildren<GtLabel*>())
    {
        if (!retval.contains(label->objectName()))
        {
            retval << label->objectName();
        }
    }

    return retval;
}

bool
GtObject::hasChanges()
{
    return (objectFlags() & GtObject::HasOwnChanges);
}

bool
GtObject::hasChildChanged()
{
    return (objectFlags() & GtObject::HasChildChanges);
}

bool
GtObject::newlyCreated()
{
    return (objectFlags() & GtObject::NewlyCreated);
}

void
GtObject::acceptChanges()
{
    m_objectFlags = m_objectFlags
                    & ~GtObject::HasOwnChanges
                    & ~GtObject::HasChildChanges
                    & ~GtObject::NewlyCreated;
}

void
GtObject::acceptChangesRecursively()
{
    acceptChanges();

    foreach (GtObject* child, findDirectChildren<GtObject*>())
    {
        child->acceptChangesRecursively();
    }
}

void
GtObject::debugObjectTree(int indent)
{
    GtDummyObject* d_obj = qobject_cast<GtDummyObject*>(this);

    if (d_obj != Q_NULLPTR)
    {
        // debug dummy properties
        foreach (GtDummyObject::DummyProperty dp,
                 d_obj->dummyProperties())
        {
            qDebug() << ": " << dp.m_id << " [" << dp.m_type << ", "
                     << dp.m_optional << ", " << dp.m_active << ", "
                     << dp.m_val << "]";
        }
    }

    QString str = QStringLiteral("|-> ");

    for (int i = 0; i < indent; i++)
    {
        str.push_front(QStringLiteral("\t"));
    }

    if (m_factory)
    {
        gtDebug() << str << objectName() << QStringLiteral(" (") <<
                  metaObject()->className() <<
                  QStringLiteral(") - ") << m_factory;
    }
    else
    {
        gtDebug() << str << objectName() << QStringLiteral(" (") <<
                  metaObject()->className() << QStringLiteral(")");
    }

    foreach (GtObject* o, findDirectChildren<GtObject*>())
    {
        o->debugObjectTree(indent + 1);
    }
}

const QList<GtAbstractProperty*>&
GtObject::properties() const
{
    return m_properties;
}

QList<GtAbstractProperty*>
GtObject::fullPropertyList() const
{
    QList<GtAbstractProperty*> retval;

    foreach (GtAbstractProperty* prop, properties())
    {
        fullPropertyListHelper(prop, retval);
    }

    return retval;
}

GtAbstractProperty*
GtObject::findProperty(const QString& id)
{
    foreach (GtAbstractProperty* property, m_properties)
    {
        if (property->ident() == id)
        {
            return property;
        }

        GtAbstractProperty* tmp = property->findProperty(id);

        if (tmp != Q_NULLPTR)
        {
            return tmp;
        }
    }

    return Q_NULLPTR;
}

GtAbstractProperty*
GtObject::findPropertyByName(const QString& name)
{
    foreach (GtAbstractProperty* property, m_properties)
    {
        if (property->objectName() == name)
        {
            return property;
        }

        GtAbstractProperty* tmp = property->findPropertyByName(name);

        if (tmp != Q_NULLPTR)
        {
            return tmp;
        }
    }

    return Q_NULLPTR;
}

void
GtObject::changed()
{
    setFlag(GtObject::HasOwnChanges);
    emit dataChanged(this);
}

QString
GtObject::objectPath(QString& str)
{
    if (!str.isEmpty())
    {
        str.insert(0, QStringLiteral(";"));
    }

    str.insert(0, objectName());

    if (parentObject() != Q_NULLPTR)
    {
        return parentObject()->objectPath(str);
    }

    return str;
}

GtObject*
GtObject::getObjectByPath(QStringList& objectPath)
{
    if (objectPath.isEmpty())
    {
        return Q_NULLPTR;
    }

    if (objectPath.size() > 1)
    {
        if (objectPath.first() == objectName())
        {
            objectPath.takeFirst();

            GtObject* child = findDirectChild<GtObject*>(objectPath.first());

            if (child == Q_NULLPTR)
            {
                return Q_NULLPTR;
            }

            return child->getObjectByPath(objectPath);
        }
        else
        {
            return Q_NULLPTR;
        }
    }

    if (objectPath.first() == objectName())
    {
        return this;
    }

    return Q_NULLPTR;
}

QString
GtObject::objectPath()
{
    QString str;

    return objectPath(str);
}

void
GtObject::setFlag(GtObject::ObjectFlag flag, bool enable)
{
    if (enable)
    {
        m_objectFlags = m_objectFlags | flag;
    }
    else
    {
        m_objectFlags = m_objectFlags & ~flag;
    }
}

GtAbstractObjectFactory*
GtObject::factory()
{
    return m_factory;
}

void
GtObject::propertyChanged(QObject* obj)
{
    setFlag(GtObject::HasOwnChanges, true);

    if (GtAbstractProperty* prop = qobject_cast<GtAbstractProperty*>(obj))
    {
        emit dataChanged(this, prop);
    }
}

void
GtObject::onChildDataChanged()
{
    setFlag(GtObject::HasChildChanges);
}

GtObject*
GtObject::getObjectByUuid(const QString& objectUUID)
{
    if (uuid() == objectUUID)
    {
        return this;
    }

    GtObject* retval = Q_NULLPTR;

    QList<GtObject*> list = findChildren<GtObject*>();

    foreach (GtObject* obj, list)
    {
        if (obj->uuid() == objectUUID)
        {
            return obj;
        }
    }

    return retval;
}

GtObject*
GtObject::getDirectChildByUuid(const QString& objectUUID)
{
    QList<GtObject*> list = findDirectChildren<GtObject*>();

    foreach (GtObject* obj, list)
    {
        if (obj->uuid() == objectUUID)
        {
            return obj;
        }
    }

    return Q_NULLPTR;
}

GtObject*
GtObject::getObjectByPath(const QString& objectPath)
{
    QStringList list = objectPath.split(QStringLiteral(";"));

    return getObjectByPath(list);
}

//void
//GtObject::changed()
//{
//    emit dataChanged(this);
//}

void
GtObject::connectProperty(GtAbstractProperty& property)
{
    connect(&property, SIGNAL(changed()), m_propertyMapper, SLOT(map()),
            Qt::UniqueConnection);

    m_propertyMapper->setMapping(&property, &property);

    foreach (GtAbstractProperty* child, property.fullProperties())
    {
        connectProperty(*child);
    }
}

void
GtObject::fullPropertyListHelper(GtAbstractProperty* p,
                                 QList<GtAbstractProperty*>& list) const
{
    // check property
    if (p == Q_NULLPTR)
    {
        return;
    }

    if (!list.contains(p))
    {
        list << p;
    }

    foreach (GtAbstractProperty* childProp, p->fullProperties())
    {
        fullPropertyListHelper(childProp, list);
    }
}

void
GtObject::newChildUUIDs(GtObject* parent)
{
    foreach(GtObject* child, parent->findChildren<GtObject*>())
    {
        child->newUuid();
    }
}

bool
GtObject::registerProperty(GtAbstractProperty& property)
{
    if (m_properties.contains(&property))
    {
        gtWarning() << tr("multiple property registration!")
                    << QStringLiteral(" Object Name (") << objectName()
                    << QStringLiteral(")")
                    << QStringLiteral(" Property Name (") << property.ident()
                    << QStringLiteral(")");
        return false;
    }

    connectProperty(property);

    m_properties.append(&property);

    return true;
}

bool
GtObject::registerProperty(GtAbstractProperty& property, const QString& cat)
{
    property.setCategory(cat);

    return registerProperty(property);
}

void
GtObject::onObjectDataMerged()
{
    // nothing to do here
}

bool
GtObject::childAccepted(GtObject* /*child*/)
{
    // accept anything
    return true;
}

namespace gt
{
void moveToThread(GtAbstractProperty* prop, QThread* thread)
{
    if (!prop || !thread) return;

    prop->QObject::moveToThread(thread);
}


void
moveToThread(GtObject& object, QThread* thread)
{
    if (!thread) return;

    // collect all child objects and add root object to list
    auto objs = object.findChildren<GtObject*>();
    objs.push_front(&object);

    for (auto* obj : qAsConst(objs))
    {
        const auto allChildProps = obj->fullPropertyList();
        // move all properties of current object to new thread
        for (auto* childProp : allChildProps)
        {
            moveToThread(childProp, thread);
        }

    }

    object.QObject::moveToThread(thread);
}

} // namespace gt
