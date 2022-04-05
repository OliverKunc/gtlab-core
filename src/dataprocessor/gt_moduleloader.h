/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 11.08.2015
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#ifndef GTMODULELOADER_H
#define GTMODULELOADER_H

#include "gt_datamodel_exports.h"

#include <QMap>
#include <QVariantList>

class QStringList;
class QJsonObject;
class QPluginLoader;
class QDir;
class GtModuleInterface;
class GtVersionNumber;

/**
 * @brief The GtModuleLoader class
 */
class GT_DATAMODEL_EXPORT GtModuleLoader
{
public:
    /**
     * @brief GtModuleLoader
     */
    GtModuleLoader();

    /**
     * @brief ~GtModuleLoader
     */
    virtual ~GtModuleLoader();

    /**
     * @brief load
     * @param ignoreCrashList - if set to true the list of crashed modules
     * will be ignored.
     *
     * Should only be used for console application
     */
    void load(bool ignoreCrashList = false);

    /**
     * @brief Returns list of all module environment variables.
     * @return List of all module environment variables.
     */
    static QMap<QString, QString> moduleEnvironmentVars();

    /**
     * @brief Returns identification strings of all loaded modules.
     * @return module identification strings
     */
    QStringList moduleIds();

    /**
     * @brief Returns identification strings of all loaded modules which
     * inherit the datamodel interface.
     * @return module identification strings
     */
    QStringList moduleDatamodelInterfaceIds();

    /**
     * @brief Returns version number of module for given id. Returns -1
     * for non existing modules.
     * @param module identification string
     * @return module version number
     */
    GtVersionNumber moduleVersion(const QString& id);

    /**
     * @brief Returns description of module for given id. Returns empty
     * string for non existing modules.
     * @param module identification string
     * @return module description
     */
    QString moduleDescription(const QString& id);

    /**
     * @brief Returns classname of package for a given module id. Returns empty
     * string for non existing modules.
     * @param module identification string
     * @return package classname
     */
    QString modulePackageId(const QString& id);

    /**
     * @brief Initializes all loaded modules.
     */
    void initModules();

protected:
    /**
     * @brief check
     * @param plugin
     * @return
     */
    virtual bool check(GtModuleInterface* plugin);

    /**
     * @brief insert
     * @param plugin
     */
    virtual void insert(GtModuleInterface* plugin);

private:
    /// Successfully loaded plugins
    QMap<QString, GtModuleInterface*> m_plugins;

    /// Modules initialized indicator.
    bool m_modulesInitialized;

    /**
     * @brief Returns plugin meta data.
     * @param loader Plugin loader.
     * @return List of plugin meta data in form of an json object.
     */
    static QJsonObject pluginMetaData(const QPluginLoader& loader);

    /**
     * @brief Returns specific meta data knot.
     * @param metaData Plugin meta data.
     * @param id Identification string of specific meta data knot.
     * @return Meta data knot.
     */
    static QVariantList metaArray(const QJsonObject& metaData,
                                  const QString& id);

    /**
     * @brief loadHelper
     * @param entries
     * @param modulesDir
     * @param ignoreModuleCrash - the list of modules which crashed before and
     * should not be loaded will be ignored - not sensefull for GUI application
     * but for GTlab console application
     * @return
     */
    bool loadHelper(QStringList& entries, const QDir& modulesDir,
                    const QStringList& excludeList,
                    bool const ignoreModuleCrash = false);

    /**
     * @brief checkDependency
     * @param mapItem
     * @return
     */
    bool checkDependency(const QVariantList& deps);

    /**
     * @brief debugDependencies
     * @param path
     */
    void debugDependencies(const QString& path);

    /**
     * @brief Returns application roaming path.
     * @return Application roaming path.
     */
    static QString roamingPath();

};

#endif // GTMODULELOADER_H
