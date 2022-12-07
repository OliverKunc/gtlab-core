/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 13.10.2015
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#ifndef GTPROJECTPROVIDER_H
#define GTPROJECTPROVIDER_H

#include "gt_core_exports.h"

#include <QObject>
#include <QPointer>
#include <QStringList>
#include <memory>

class GtProject;
class GtAbstractProperty;
class GtObjectLinkProperty;
class GtObject;
class GtPropertyConnection;

class QDir;

/**
 * @brief The GtProjectProvider class
 */
class GT_CORE_EXPORT GtProjectProvider : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief GtProjectProvider
     * @param parent
     */
    explicit GtProjectProvider(QObject* parent = nullptr);

    /**
     * @brief GtProjectProvider
     * @param projectFileName, e.g. "MyProjectDir/project.gtlab"
     * @param parent
     */
    explicit GtProjectProvider(const QString& projectFilePath,
                               QObject* parent = nullptr);

    /**
     * @brief GtProjectProvider
     * @param project
     * @param parent
     */
    explicit GtProjectProvider(GtProject* project,
                               QObject* parent = nullptr);

    /**
     * @brief setSource
     * @param project
     */
    void setSource(GtProject* project);

    /**
     * @brief project
     * @return
     */
    GtProject* project();

    /**
     * @brief Duplicates the currently opened project and saves it
     *        to a new location (used for save as)
     *
     * Note: instead of duplicate, we should change this into saveAs
     *
     * @param newId
     * @param newPath
     * @return
     */
    GtProject* duplicateProject(const QString& newId, const QString& newPath);

    /**
     * @brief Duplicates a closed project into a new path with a new project name
     *
     *
     * @param projectPath
     * @param newProjectPath
     * @param newProjectName
     * @return
     */
    static std::unique_ptr<GtProject> duplicateExistingProject(
        const QDir& projectPath,
        const QDir& newProjectPath,
        const QString& newProjectName);

    /**
     * @brief apply
     * @return
     */
    bool apply();

    /**
     * @brief projectName
     * @return
     */
    const QString& projectName();

    /**
     * @brief setProjectName
     * @param val
     */
    void setProjectName(const QString& val);

    /**
     * @brief projectPath
     * @return
     */
    const QString& projectPath();

    /**
     * @brief setProjectPath
     * @param path
     */
    void setProjectPath(const QString& path);

    /**
     * @brief projectModules
     * @return
     */
    const QStringList& projectModules();

    /**
     * @brief setProjectModules
     * @param val
     */
    void setProjectModules(const QStringList& val);

    /**
     * @brief projectFileExists
     * @param path
     * @return
     */
    static bool projectFileExists(const QString& path);

    /**
     * @brief projectExistsInSession
     * @param id
     * @return
     */
    static bool projectExistsInSession(const QString& id);

    /**
     * @brief moduleFilePath
     * @param id
     * @return
     */
    QString moduleFilename(const QString& id);

private:
    /// project name
    QString m_pName;

    /// project path
    QString m_pPath;

    /// project modules
    QStringList m_pModules;

    /// temporary project
    QPointer<GtProject> m_project;

    /**
     * @brief mapFromSource
     * @param project
     */
    void mapFromSource();

    /**
     * @brief mapToSource
     */
    void mapToSource();

    /**
     * @brief generateFiles
     * @param path
     * @return
     */
    bool generateFiles();

    /**
     * @brief modifyFiles
     * @return
     */
    bool modifyFiles();

    /**
     * @brief generateMainProjectFile
     * @return
     */
    bool generateMainProjectFile();

    /**
     * @brief modifyMainProjectFile
     * @return
     */
    bool modifyMainProjectFile();

    /**
     * @brief generateModuleFiles
     * @return
     */
    bool generateModuleFiles();

    /**
     * @brief generateModuleFile
     * @param id
     * @return
     */
    bool generateModuleFile(const QString& id);

    /**
     * @brief objecgtLinkProperties
     * @param obj
     * @return
     */
    QList<GtObjectLinkProperty*> objectLinkProperties(GtObject* obj);

    /**
     * @brief fullPropertyList
     * @param obj
     * @return
     */
    QList<GtAbstractProperty*> fullObjectPropertyList(GtObject* obj);

    /**
     * @brief fullPropertyPropertyList
     * @param obj
     * @return
     */
    QList<GtAbstractProperty*> fullPropertyList(GtAbstractProperty* p);

    /**
     * @brief resetUuid
     * @param obj
     * @param mapping
     */
    void resetUuid(GtObject* obj, QMap<QString, QString>& mapping);

};

#endif // GTPROJECTPROVIDER_H
