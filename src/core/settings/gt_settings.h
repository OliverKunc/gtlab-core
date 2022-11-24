/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 30.07.2015
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#ifndef GTSETTINGS_H
#define GTSETTINGS_H

#include "gt_core_exports.h"
#include "gt_abstractsettings.h"
#include "gt_shortcutsettingsdata.h"

/**
 * @brief The GtSettings class
 */
class GT_CORE_EXPORT GtSettings : public GtAbstractSettings
{
public:
    /**
     * @brief GtSettings
     */
    GtSettings();

    ~GtSettings() override;

    /**
     * @brief lastSession
     * @return
     */
    QString lastSession();

    /**
     * @brief setLastSession
     * @param val
     */
    void setLastSession(const QString& session);

    /**
     * @brief lastPerspective
     * @return
     */
    QString lastPerspective();

    /**
     * @brief setLastPerspective
     * @param val
     */
    void setLastPerspective(const QString& perspective);

    /**
     * @brief lastPath
     * @return
     */
    QString lastPath();

    /**
     * @brief setLastPath
     * @param val
     */
    void setLastPath(const QString& path);

    /**
     * @brief openLastSession
     * @return
     */
    bool openLastSession();

    /**
     * @brief setOpenLastSession
     * @param val
     */
    void setOpenLastSession(bool value);

    /**
     * @brief language
     * @return
     */
    QString language();

    /**
     * @brief setLanguage
     * @param val
     */
    void setLanguage(const QString& language);

    /**
     * @brief firstApplicationRun
     * @return
     */
    bool firstApplicationRun();

    /**
     * @brief setShowStartupPage
     * @param val
     */
    void setShowStartupPage(bool value);

    /**
     * @brief showStartupPage
     * @return
     */
    bool showStartupPage();

    /**
     * @brief setSearchForUpdate
     * @param val
     */
    void setSearchForUpdate(bool value);

    /**
     * @brief searchForUpdate
     * @return
     */
    bool searchForUpdate();

    /**
     * @brief lastProject
     * @return
     */
    QString lastProject();

    /**
     * @brief setLastProject
     * @param val
     */
    void setLastProject(const QString& project);

    /**
     * @brief openLastProject
     * @return
     */
    bool openLastProject();

    /**
     * @brief setOpenLastProject
     * @param val
     */
    void setOpenLastProject(bool value);

    /**
     * @brief setMaxLogLength
     * @param val
     */
    void setMaxLogLength(int value);

    /**
     * @brief maxLogLength
     * @return
     */
    int maxLogLength();

    /**
     * @brief Setter for the logging verbosity (see gt::log::Verbosity)
     * @param value
     */
    void setLoggingVerbosity(int value);

    /**
     * @brief Getter for the logging verbosity (see gt::log::Verbosity)
     * @return Verbosity
     */
    int loggingVerbosity() const;

    /**
     * @brief Sets last used process elements.
     * @param list List of last used process element class names.
     */
    void setLastProcessElements(const QStringList& list);

    /**
     * @brief Returns list of last used process elements.
     * @return List of last used process element class names.
     */
    QStringList lastProcessElements();

    /**
     * @brief Stores given expand state list of explorer dock widget.
     * @param list List of expand states.
     */
    void setExplorerExpandStates(const QStringList& list);

    /**
     * @brief Returns expand states of explorer dock widget.
     * @return List including expand states of explorer dock widget.
     */
    QStringList explorerExpandStates();

    /**
     * @brief shortcutsList
     * @return list of shortcut definitions
     */
    QList<GtShortCutSettingsData> shortcutsList() const;

    /**
     * @brief setShortcutsTable
     * @param list - list to set in the settings
     */
    void setShortcutsTable(QList<GtShortCutSettingsData> const& list);

    /**
     * @brief initialShortCuts
     * @return the default shortcuts as a variant (QMap<QString, QStringList>).
     * QStringList contains two elements (keysequence and category)
     */
    QVariant initialShortCuts() const;

    /**
     * @brief intialShortCutsList
     * @return list of default shortcuts as a list
     */
    QList<GtShortCutSettingsData> intialShortCutsList() const;

    QString themeMode();

    bool darkMode();

    void setThemeMode(const QString& theme);

    /**
     * @brief Returns whether the extended process executor is enabled
     * @return Is extended process executor enabled
     */
    bool useExtendedProcessExecutor() const;

    /**
     * @brief Setter for the extended process executor
     * @param value Value
     */
    void setUseExtendedProcessExecutor(bool value);

    /**
     * @brief Whether to autostart the process runner
     * @return Autostart
     */
    bool autostartProcessRunner() const;

    /**
     * @brief Setter for the autostart property
     * @param value Value
     */
    void setAutostartProcessRunner(bool value);

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

#endif // GTSETTINGS_H
