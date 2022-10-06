/* GTlab - Gas Turbine laboratory
 * Source File:
 * copyright 2009-2015 by DLR
 *
 *  Created on: 24.07.2015
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include <iostream>

#include <QApplication>
#include <QDomDocument>
#include <QObject>
#include <QDir>
#include <QDebug>

#include "gt_coreapplication.h"
#include "gt_coredatamodel.h"
#include "gt_coreprocessexecutor.h"
#include "gt_project.h"
#include "gt_projectprovider.h"
#include "internal/gt_commandlinefunctionhandler.h"
#include "gt_task.h"
#include "gt_footprint.h"
#include "batchremote.h"
#include "gt_consoleparser.h"
#include "gt_versionnumber.h"

using namespace std;

void
showSplashScreen()
{
    cout << endl;
    cout << "******************************************" << endl;
    cout << "    _____________________      ______  " << endl;
    cout << "    __  ____/__  __/__  /_____ ___  /_ " << endl;
    cout << "    _  / __ __  /  __  /_  __ `/_  __ \\" << endl;
    cout << "    / /_/ / _  /   _  / / /_/ /_  /_/ /" << endl;
    cout << "    \\____/  /_/    /_/  \\__,_/ /_.___/ " << endl;
    cout << "                                 "
         << GtCoreApplication::versionToString().toStdString() << endl;
    cout << "******************************************" << endl;
    cout << endl;
}

int
checkMetaInput(const QStringList& args)
{
    GtCommandLineParser p;
    p.parse(args);

    if (p.positionalArguments().size() != 1)
    {
        qWarning() << QStringLiteral("ERROR: ")
                   << QObject::tr("Invalid size of arguments for check meta!"
                                  "Exactly one argument is required");
        return -1;
    }

    QString fileName = p.positionalArguments().constFirst();

    qDebug() << "meta input check...";

    if (fileName.isEmpty())
    {
        qWarning() << QStringLiteral("ERROR: ")
                   << QObject::tr("file name is empty!");
        return -1;
    }

    if (!BatchRemote::checkInput(fileName, GtCoreApplication::versionToString(),
                                 false, false))
    {
        qWarning() << QStringLiteral("ERROR: ")
                   << QObject::tr("meta input invalid!");
        return -1;
    }

    qDebug() << "meta input ok!";
    return 0;
}

int
runMetaInput(const QStringList& args)
{
    GtCommandLineParser p;
    p.parse(args);

    if (p.positionalArguments().size() < 2)
    {
        qWarning() << "Invalid arguments for runMetaInput.";
        qWarning() << "Two arguments needed as <Input> <Output>";

        return -1;
    }


    QString inputFileName = p.positionalArguments().constFirst();
    QString outputFileName = p.positionalArguments().at(1);

    qDebug() << "meta input run...";

    if (inputFileName.isEmpty())
    {
        qWarning() << QStringLiteral("ERROR: ") <<
                   QObject::tr("input file name is empty!");
        return -1;
    }

    if (outputFileName.isEmpty())
    {
        qWarning() << QStringLiteral("ERROR: ") <<
                   QObject::tr("output file name is empty!");
        return -1;
    }

    qDebug() << "   input file name: " << inputFileName;
    qDebug() << "   output file name: " << outputFileName;

    int inputCheck = checkMetaInput({inputFileName});

    if (inputCheck != 0)
    {
        qWarning() << QStringLiteral("ERROR: ") <<
                   QObject::tr("meta input run failed!");
        return inputCheck;
    }

    qDebug() << "executing process...";

    if (!BatchRemote::run(inputFileName, outputFileName,
                          GtCoreApplication::versionToString()))
    {
        qWarning() << QStringLiteral("ERROR: ") <<
                   QObject::tr("process execution failed!");
        return -1;
    }

    qDebug() << "meta input run successful!";

    return 0;
}

int
runProcess(const QString& projectId, const QString& processId,
           bool save = false)
{
    qDebug() << "process run...";

    if (projectId.isEmpty())
    {
        qWarning() << QStringLiteral("ERROR: ") <<
                   QObject::tr("project id is empty!");

        return -1;
    }

    if (processId.isEmpty())
    {
        qWarning() << QStringLiteral("ERROR: ") <<
                   QObject::tr("process id is empty!");

        return -1;
    }

    GtProject* project = gtApp->findProject(projectId);

    if (!project)
    {
        qWarning() << QStringLiteral("ERROR: ") <<
                   QObject::tr("project not found!") <<
                   QStringLiteral(" (") << projectId << QStringLiteral(")");

        return -1;
    }

    if (!gtDataModel->GtCoreDatamodel::openProject(project))
    {
        qWarning() << QStringLiteral("ERROR: ") <<
                   QObject::tr("could not open project!") <<
                   QStringLiteral(" (") << projectId << QStringLiteral(")");

        return -1;
    }

    qDebug() << "project opened!";

    // run process
    GtTask* process = project->findProcess(processId);

    if (!process)
    {
        qWarning() << QStringLiteral("ERROR: ") <<
                   QObject::tr("process not found!") <<
                   QStringLiteral(" (") << processId << QStringLiteral(")");

        return -1;
    }

    // execute process
    GtCoreProcessExecutor executor;
    executor.runTask(process);

    if (process->currentState() != GtProcessComponent::FINISHED)
    {
        qWarning() << "Calculator run failed!";
        return -1;
    }

    qDebug() << "process run successful!";

    if (save)
    {
        if (!gtDataModel->saveProject(project))
        {
            qWarning() << QStringLiteral("ERROR: ") <<
                       QObject::tr("project could not besaved!") <<
                       QStringLiteral(" (") << projectId <<
                       QStringLiteral(")");
            return -1;
        }
    }

    return 0;
}


int
runProcessByFile(const QString& projectFile, const QString& processId,
                 bool save = false)
{
    qDebug() << "process run...";

    if (projectFile.isEmpty())
    {
        qWarning() << QStringLiteral("ERROR: ")
                   << QObject::tr("project file is empty!");

        return -1;
    }

    if (processId.isEmpty())
    {
        qWarning() << QStringLiteral("ERROR: ")
                   << QObject::tr("process id is empty!");

        return -1;
    }

    QFile file(projectFile);

    if (!file.exists())
    {
        qWarning() << QStringLiteral("ERROR: ")
                   << QObject::tr("project file")
                   << projectFile
                   << QObject::tr("not found!");

        return -1;
    }
    //gtDebug() << "  |-> is existing file!";

    GtProjectProvider provider(projectFile);
    GtProject* project = provider.project();

    if (!project)
    {
        gtError() << "Cannot load project";
        return -1;
    }   

    gtApp->session()->appendChild(project);

    if (!gtDataModel->GtCoreDatamodel::openProject(project))
    {
        qWarning() << QStringLiteral("ERROR: ")
                   << QObject::tr("could not open project!")
                   << QStringLiteral(" (") << projectFile
                   << QStringLiteral(")");

        return -1;
    }

    qDebug() << "project opened!";

    // run process
    GtTask* process = project->findProcess(processId);

    if (!process)
    {
        qWarning() << QStringLiteral("ERROR: ")
                   << QObject::tr("process not found!")
                   << QStringLiteral(" (") << processId << QStringLiteral(")");

        return -1;
    }

    // execute process
    GtCoreProcessExecutor executor(nullptr, false);
    executor.runTask(process);

    if (process->currentState() != GtProcessComponent::FINISHED)
    {
        qWarning() << "Calculator run failed!";
        return -1;
    }

    qDebug() << "process run successful!";

    if (save)
    {
        if (!gtDataModel->saveProject(project))
        {
            qWarning() << QStringLiteral("ERROR: ")
                       << QObject::tr("project could not besaved!")
                       << QStringLiteral(" (") << projectFile
                       << QStringLiteral(")");
            return -1;
        }
    }

    return 0;
}

void
printRunHelp()
{
    std::cout << std::endl;
    std::cout << "This is the help for the GTlab run function" << std::endl;
    std::cout << std::endl;

    std::cout << "There are two basic methods to start a process:" << std::endl;
    std::cout << "\tDefine the project by name from the current session"
                 "(default option or --name or -n)" << std::endl;
    std::cout  << "\tGTlabConsole.exe run -f <projectName> <processname> [-s] "
               << std::endl;

    std::cout << std::endl;
    std::cout << "\tDefine the project by file (use the option --file or -f"
              << std::endl;
    std::cout << "\tGTlabConsole.exe run [-n] <fileName> <processname> [-s]  "
              << std::endl;

    std::cout << std::endl;

    std::cout << "\tAdditionally you can set the option -s or --save"
              << std::endl;
    std::cout << "\tWith this option the results of the successfull process are"
              << " saved in the datamodel" << std::endl;

    std::cout << std::endl;
}

int
run(QStringList const& args)
{
    GtCommandLineParser p;
    p.addHelpOption();
    p.addOption("save", {"save", "s"},
                "save the process result after successfull run");
    p.addOption("file", {"file", "f"},
                "file to gtlab project");
    p.addOption("name", {"name", "n"},
                "name of project in current session");

    if (!p.parse(args))
    {
        qWarning() << "Run method without arguments is invalid";
        return -1;
    }

    if (p.helpOption())
    {
        printRunHelp();
        return 0;
    }

    bool save = false;

    if (p.option("save"))
    {
        save = true;
        std::cout << "Activate save option" << std::endl;
    }

    if (p.option("file"))
    {
        if (p.positionalArguments().size() != 2)
        {
            qWarning() << "Invalid usage of file option";
            return -1;
        }

        return runProcessByFile(p.positionalArguments().at(0),
                p.positionalArguments().at(1), save);
    }

    if (p.option("name"))
    {
        if (p.positionalArguments().size() != 2)
        {
            qWarning() << "Invalid usage of name option";
            return -1;
        }

        return runProcess(p.positionalArguments().at(0),
                p.positionalArguments().at(1), save);
    }

    return -1;
}

int
showFootprint(const QStringList& args)
{
    Q_UNUSED(args)
    std::cout << GtFootprint().exportToXML().toStdString() << std::endl;

    return 0;
}

void
initPosArgument(QString const& id,
                std::function<int(const QStringList&)> func,
                QString const& brief,
                QList<GtCommandLineOption> const& options = {},
                QList<GtCommandLineFunctionArgument> const& args = {},
                bool defaultHelp = true)
{
    auto fun = GtCommandLineInterface::make_commandLineFunction(
                id, func, brief);
    fun.setOptions(options)
            .setArgs(args)
            .setUseDefaultHelp(defaultHelp);

    gtlab::commandline::register_function(fun);
}

void
initSystemOptions()
{
    /// Add options for positional arguments to the command line interface
    initPosArgument("footprint", showFootprint, "Displays framework footprint");
    initPosArgument("check_meta", checkMetaInput,
                    "Checks given meta process data."
                    "\n\t\t\tUsage: check_meta <input.xml>");
    initPosArgument("run_meta", runMetaInput,
                    "Executes given meta process data. "
                    "Results are stored in given output file."
                    "\n\t\t\tUsage; run_meta <input.xml> <output.xml>");
    QList<GtCommandLineOption> runOptions;
    runOptions.append(GtCommandLineOption{
                          {"save", "s"},
                          "Saves datamodel after successfull process run"});
    runOptions.append(GtCommandLineOption{
                          {"name", "n"}, "Define project by name"});
    runOptions.append(GtCommandLineOption{
                          {"file", "f"}, "Define project by file"});
    initPosArgument("run", run,
                    "\tExecutes a process. \n\t\t\t"
                    "To define a project name and a process name is the "
                    "default used option to execute this command."
                    "\n\t\t\tUse --help for more details.",
                    runOptions,
                    QList<GtCommandLineFunctionArgument>(),
                    false);
}

int
initModuleTest(QStringList const& arguments, GtCoreApplication& app)
{
    GtCommandLineParser p;
    p.parse(arguments);

    qDebug() << "Start testing to load module file";

    if (p.positionalArguments().isEmpty())
    {
        qWarning() << QStringLiteral("ERROR: ")
                   << QObject::tr("invalid arguments");
        return -1;
    }

    // extract path to the module to load
    QString moduleToLoad = p.positionalArguments().constFirst();

    qDebug() << "Check module:" << moduleToLoad;

    // load GTlab modules
    app.loadModules();

    // calculator initialization
    app.initCalculators();

    // initialize modules
    app.initModules();

    // check if module is loaded
    qDebug() << "Use footprint as a first test";
    std::cout << GtFootprint().exportToXML().toStdString() << endl;

    return -1;
}


int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationDomain("www.dlr.de");
    QCoreApplication::setOrganizationName("DLR");
    QCoreApplication::setApplicationName("GTlab");

    QCoreApplication::setApplicationVersion(
                GtCoreApplication::versionToString());

    QApplication a(argc, argv);

    QStringList args = a.arguments();

    if (args.isEmpty())
    {
        return -1;
    }
    /// Remove the function name itself from arguments
    args.removeFirst();

    showSplashScreen();

    // Setup parser for main function
    GtConsoleParser parser;
    parser.addOption("dev",
                     {QStringLiteral("dev")},
                     "Activate the developer mode");
    parser.addHelpOption();
    parser.addOption("session",
                     {"session", "se"},
                     "Defines a session to be used for "
                     "execution."
                     "\n\t\t\tUsage: --session <session_id>");
    parser.addOption("version",
                     {"version", "v"},
                     "\tDisplays the version number of GTlab");

    if (!parser.parse(args))
    {
        std::cout << "Parsing arguments failed" << std::endl;
        return -1;
    }

    /// Add options for positional arguments to the command line interface
    initSystemOptions();

    // application initialization
    GtCoreApplication app(qApp, GtCoreApplication::AppMode::Batch);

    // Load module option
    if (parser.option("dev"))
    {
        app.setDevMode(true);
    }

    // Load module option
    if (parser.option("version"))
    {
        std::cout << "GTlab version:" << std::endl;
        std::cout << app.version().toString().toStdString() << std::endl;
        return 0;
    }

    app.init();

    // save to system environment (temporary)
    app.saveSystemEnvironment();

    // language initialization
    app.initLanguage();

    // datamodel initialization
    app.initDatamodel();

    // Load module option
    if (parser.argument("load_module"))
    {
        return initModuleTest(parser.arguments(), app);
    }

    // load GTlab modules
    app.loadModules();

    // calculator initialization
    app.initCalculators();

    // session initialization
    if (parser.option("session"))
    {
        QString sessionValue = parser.optionValue("session").toString();
        std::cout << "Select session: " << sessionValue.toStdString()
                  << std::endl;
        app.initSession(sessionValue);
    }
    else
    {
        app.initSession();
    }

    if (!app.session())
    {
        qWarning() << "no session loaded!";
        return -1;
    }

    // initialize modules
    app.initModules();

    QStringList commands =
            GtCommandLineFunctionHandler::instance().getRegisteredFunctionIDs();

    for (QString const& s: qAsConst(commands))
    {
        GtCommandLineFunction f =
                GtCommandLineFunctionHandler::instance().getInterfaceFunc(s);
        parser.addPositionalArgument(f);
    }

    QString mainArg = parser.firstPositionalArgument();
    if (mainArg.isEmpty())
    {
        /// show main application help if it is requested
        if (parser.helpOption())
        {
            parser.showHelp();
            return 0;
        }

        qWarning() << "No valid argument could be found in the arguments:";
        parser.debugArguments();
        return -1;
    }

    if (commands.contains(mainArg))
    {
        GtCommandLineFunction f =
                GtCommandLineFunctionHandler::instance().getInterfaceFunc(
                    mainArg);

        /// check if the default help flag is part of the arguments
        if (parser.helpOption())
        {
            if (f.useDefaultHelp())
            {
                f.showDefaultHelp();
                return 0;
            }
        }

        /// remove the argument which lead to this function call
        parser.removeArg(mainArg);

        /// if a customHelpIs
        return f(parser.arguments());
    }
    else
    {
        qCritical() << "Invalid command" << mainArg;
        return -1;
    }

    qWarning() << QObject::tr("invalid arguments! "
                              "use run --help for further information.");

    parser.showHelp();

    return -1;
}
