/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 07.08.2015
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QInputDialog>
#include <QDebug>

#include "gt_preferencessession.h"

#include "gt_coreapplication.h"
#include "gt_settings.h"
#include "gt_sessionlistwidget.h"
#include "gt_processexecutor.h"

GtPreferencesSession::GtPreferencesSession()
{
    setTitle(tr("Session"));

    QVBoxLayout* lay = new QVBoxLayout;

    QHBoxLayout* hLayout = new QHBoxLayout;

    m_list = new GtSessionListWidget(false);

    connect(m_list, SIGNAL(itemSelectionChanged()),
            SLOT(onItemSelectionChange()));

    hLayout->addWidget(m_list);

    QVBoxLayout* bLayout = new QVBoxLayout;

    m_btnNew = new QPushButton(tr("New"));

    m_btnRename = new QPushButton(tr("Rename"));
    m_btnDuplicate = new QPushButton(tr("Duplicate"));

    m_btnDelete = new QPushButton(tr("Delete"));

    m_btnSwitch = new QPushButton(tr("Switch to"));

    bLayout->addWidget(m_btnNew);
    bLayout->addWidget(m_btnRename);
    bLayout->addWidget(m_btnDuplicate);
    bLayout->addWidget(m_btnDelete);
    bLayout->addWidget(m_btnSwitch);
    bLayout->addStretch(1);

    hLayout->addLayout(bLayout);

    lay->addLayout(hLayout);

    m_openLastCheck = new QCheckBox(tr("Open last session on startup"));

    m_openLastCheck->setChecked(gtApp->settings()->openLastSession());

    lay->addWidget(m_openLastCheck);

    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(layout());

    connect(m_btnSwitch, SIGNAL(clicked(bool)), SLOT(switchSession()));
    connect(m_btnRename, SIGNAL(clicked(bool)), m_list,
            SLOT(renameItem()));
    connect(m_btnNew, SIGNAL(clicked(bool)), m_list, SLOT(newItem()));
    connect(m_btnDuplicate, SIGNAL(clicked(bool)), m_list,
            SLOT(duplicateItem()));
    connect(m_btnDelete, SIGNAL(clicked(bool)), m_list, SLOT(deleteItem()));

    if (mainLayout)
    {
        mainLayout->addLayout(lay);
        m_list->init();
    }
    else
    {
        delete lay;
    }
}

void
GtPreferencesSession::saveSettings()
{
    gtApp->settings()->setOpenLastSession(m_openLastCheck->isChecked());
}

void
GtPreferencesSession::loadSettings()
{

}

void
GtPreferencesSession::newSession()
{

}

void
GtPreferencesSession::deleteSession()
{

}

void
GtPreferencesSession::switchSession()
{
    QListWidgetItem* item = m_list->currentItem();

    if (item == nullptr)
    {
        return;
    }

    gtApp->switchSession(item->text());
    m_list->init();
}

void
GtPreferencesSession::renameSession()
{
    QListWidgetItem* item = m_list->currentItem();

    if (item == nullptr)
    {
        return;
    }

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    m_list->editItem(item);
}

void
GtPreferencesSession::onItemSelectionChange()
{
    QListWidgetItem* item = m_list->currentItem();

    if (item == nullptr)
    {
        return;
    }

    if (item->text() == gtApp->sessionId())
    {
        m_btnSwitch->setEnabled(false);
        m_btnRename->setEnabled(false);
        m_btnDelete->setEnabled(false);
    }
    else
    {
        if (gtProcessExecutor->taskCurrentlyRunning())
        {
            m_btnSwitch->setEnabled(false);
        }
        else
        {
            m_btnSwitch->setEnabled(true);
        }

        m_btnRename->setEnabled(true);
        m_btnDelete->setEnabled(true);
    }

    if (item->text() == QLatin1String("default"))
    {
        m_btnRename->setEnabled(false);
        m_btnDelete->setEnabled(false);
    }
}

void
GtPreferencesSession::onItemTextChange(const QString& val)
{
    qDebug() << "GtPreferencesSession::onItemTextChange: " << val;
}

void
GtPreferencesSession::onLstItemsCommitData(QWidget* pLineEdit)
{
    QString strNewText = reinterpret_cast<QLineEdit*>(pLineEdit)->text();
    qDebug() << strNewText;
}

