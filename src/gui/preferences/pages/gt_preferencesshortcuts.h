/* GTlab - Gas Turbine laboratory
 * Source File: gt_preferencesshortcuts.h
 * copyright 2009-2021 by DLR
 *
 *  Created on: 13.08.2021
 *  Author: Jens Schmeink (AT-TWK)
 *  Tel.: +49 2203 601 2191
 */
#ifndef GTPREFERENCESSHORTCUTS_H
#define GTPREFERENCESSHORTCUTS_H

#include "gt_preferencespage.h"

class QTableWidget;

/**
 * @brief The GtPreferencesShortCuts class
 * Page for the preference wizard to set the short cuts for GTlab
 */
class GtPreferencesShortCuts : public GtPreferencesPage
{
    Q_OBJECT
public:
    /**
     * @brief GtPreferencesShortCuts
     * Constructor to build the widget and initialize the lines
     * of the table widget
     */
    GtPreferencesShortCuts();

    /**
     * @brief saveSettings
     */
    void saveSettings() override;

private:
    /**
     * @brief loadSettings - empty
     */
    void loadSettings() override;

    /// table widget of the main layout
    QTableWidget* m_tab;
};

#endif // GTPREFERENCESSHORTCUTS_H
