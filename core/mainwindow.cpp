/*
 *  Copyright (c) 2009 Natale Patriciello <kronat@tiscali.it>
 *
 *  This file is part of NATManagementSoftware.
 *
 *   NATManagementSw is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   NATManagementSoftware is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NATManagementSoftware.If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "plugininterface.h"
#include "stackedwcontainer.h"
#include "ui_mainwindow.h"
#include "ui_about.h"

#include <simplequery.h>
#include <simpleplainquery.h>

#include <QMessageBox>
#include <QPluginLoader>
#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QProgressDialog>

/** @brief MainWindow constructor
  * The constructor takes a QWidget as parameter, and use it as a parent
  * for the window's constructor. Then call the loader function for plugins.
  *
  * @param parent Parent widget of MainWindow ( in most cases 0 )
  * @see loadApp
  */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName("nat");
    QCoreApplication::setOrganizationDomain("nat.vacau.com");
    QCoreApplication::setApplicationName("MainWindow");
    QCoreApplication::setApplicationVersion("0.5.0");

    ui->setupUi(this);
    setWindowTitle(QCoreApplication::applicationName()+ " " +
                   QCoreApplication::applicationVersion());

    QWidget *mainWidget = loadApp(ui->menuBar, ui->mainToolBar);

    if (unlikely (mainWidget == 0))
        return;

    mainWidget->setParent(this);

    setCentralWidget(mainWidget);
}

/** @brief MainWindow deconstructor
  * Delete the ui, the tabWidget and the settingWindow.
  */
MainWindow::~MainWindow()
{
    delete ui;
    delete settingWindow;
}

/** @brief Show a warning message
  * This function shows a warning message, using QMessageBox.
  * @param generalText General information about the error
  * @param informativeText More descriptive text about the error
  */
void MainWindow::showWarning(const QString& generalText,
                             const QString& informativeText) const
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(generalText);
    msgBox.setInformativeText(informativeText);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

/** @brief Load application-plugins
  *
  * The function loadApp loads only one application-plugin that the function
  * find in the Plugins (OS-Dependent) directory.\n
  * Note that only Linux and OS X are supported.Windows porting is in
  * progress.
  *
  * @param menuBar QMenuBar for displaying plugin's menu.
  * @return The main QWidget retrieved by the application.
  * @see appInterface::setupDatabase
  * @see appInterface::setupUi
  */
QWidget* MainWindow::loadApp(QMenuBar* menuBar, QToolBar *toolBar)
{
    Q_UNUSED(menuBar);
    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->setMovable(true);
    connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(pageChanged(int)));

    QDir pluginsDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
    }
#endif // Q_OS_MAC

    pluginsDir.cd("Plugins");


    foreach (QString fileName,
                pluginsDir.entryList(QDir::NoSymLinks | QDir::Files)) {

         QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
         QObject *plugin = pluginLoader.instance();
         pluginInterface *child = qobject_cast<pluginInterface *>(plugin);

         if (likely(child != 0)) {
             QString family = child->family();

             if (m_fam.contains(family)) {
                stackedWContainer *cont = m_fam.value(family);
                cont->add(child);
             } else {
                stackedWContainer *cont = new stackedWContainer(tabWidget);
                tabWidget->addTab(cont,family);
                m_fam.insert(family,cont);
             }

             child->registerToolBar(toolBar);
             m_plugins.append(child);
         } else {
             QMessageBox msgBox;
             msgBox.setIcon(QMessageBox::Warning);
             msgBox.setText(tr("Error loading a plugin"));
             msgBox.setInformativeText(pluginLoader.errorString());
             msgBox.setStandardButtons(QMessageBox::Ok);

             msgBox.exec();
         }
     }

    m_pluginToClose = m_plugins.at(0);

    return tabWidget;
}

/** @brief Slot for closing active plugin when the page is changed
  *
  * @param index Index of the new page
  */
void MainWindow::pageChanged(int index)
{
    m_pluginToClose = m_plugins.at(index);

    m_fam.value(m_pluginToClose->family())->open();
}

/** @brief Open the settings window
  *
  * This is a slot for a menu action that open a new window, allowing the
  * user to set some important application specific parameters like
  * database host/port, database username/password.. etc.
  * @see Settings
  * @see settingsUi
  */
void MainWindow::openSettings()
{
    if (settingWindow == 0)
        settingWindow = new settingsUi(this);

    settingWindow->show();
}

/** @brief Display an about dialog
  *
  */
void MainWindow::about()
{
    Ui::AboutUi *about = new Ui::AboutUi;
    QDialog *box = new QDialog(this);
    about->setupUi(box);
    about->labelVersion->setText("Enjoy with " +
                                 QCoreApplication::applicationName() +
                                 " " + QCoreApplication::applicationVersion());
    about->labelIcon->setPixmap(windowIcon().pixmap(32,32,QIcon::Normal,QIcon::On));
    about->labelIcon->setEnabled(true);

    box->exec();

    delete about;
    delete box;
}

/** @brief Dump the database in a file
  *
  */
void MainWindow::dump()
{
    QString file;
    QString dir = QFileDialog::getExistingDirectory(this,
        tr("Select output directory"), "~", QFileDialog::ShowDirsOnly);

    if (unlikely(dir.isEmpty())) {
        showWarning(tr("No directory selected"),tr("The dump isn't started"));
        return;
    }

    QProcess pgdump;
    pgdump.setProcessChannelMode(QProcess::MergedChannels);
    pgdump.setStandardOutputFile(dir+"/MainWindow.sql");

    connect(&pgdump,SIGNAL(finished(int,QProcess::ExitStatus)),
            this,SLOT(dumpFinished(int,QProcess::ExitStatus)));

    QStringList arg;

    Settings sett;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PGPASSWORD", sett.getDbPassword());
    pgdump.setProcessEnvironment(env);

    arg << "-a"<< "--inserts"<< "-h" << sett.getDbHost() << "-p" << sett.getDbPort();
    arg << "-U" << sett.getDbUser() << sett.getDbName();

    pgdump.start("pg_dump", arg);

    if (!pgdump.waitForStarted()) {
#ifdef Q_OS_MAC
        file = QFileDialog::getOpenFileName(this,tr("Select the pg_dump program"),
                                            "/Library/PostgreSQL");
#else /* Q_OS_MAC */
        file = QFileDialog::getOpenFileName(this,tr("Select the pg_dump program"),
                                            "");
#endif /* Q_OS_MAC */
        if (file.isEmpty()) {
            showWarning(tr("No program selected"),tr("The dump isn't started"));
            return;
        }
        pgdump.start(file,arg);
    }

    pgdump.waitForFinished(1000000);
}

/** @brief Show a popup for the finished dump
  *
  */
void MainWindow::dumpFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    QMessageBox msgBox;

    msgBox.setInformativeText(tr("Dump has finished"));
    if (likely(exitStatus == QProcess::NormalExit))
        msgBox.setDetailedText(tr("The status is: Normal Exit. You now have the"
                               " file MainWindow.sql in the directory you "
                               "selected."));
    else
        msgBox.setDetailedText(tr("The status is: Crash. Dump command has crashed"
                               ", you doesn't have the MainWindow.sql file. "
                               "Please, tell to the author."));

    msgBox.setIcon(QMessageBox::Information);
}

/** @brief Setup the database
  *
  * Try to connect with the default parameters. If it fail, it ask for
  * a new installation. If it is ok, check the database for upgrade operation.
  * @see connectDefault
  * @see installDBFromZero
  * @see checkDB
  */
bool MainWindow::setupDatabase()
{
    QMessageBox msgBox;

    try {
        connectDefault();
    } catch ( QString errorMessage ) {
        // Ask for a new installation
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("Database doesn't exist"));
        msgBox.setInformativeText(tr("Do you want to try a new installation?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);

        int ret = msgBox.exec();

        if (likely(ret == QMessageBox::Yes)) // New Installation
            return installDBFromZero();
        else
            return false;
    }

    return checkDB();
}

/** @brief Try to connect with default login parameters
  *
  * @see Connection::doConnect
  */
void MainWindow::connectDefault()
{
    Settings sett;

    m_con.setDBName(sett.getDbName());
    m_con.setHost(sett.getDbHost());
    m_con.setPassword(sett.getDbPassword());
    m_con.setPort(sett.getDbPort().toInt());
    m_con.setUser(sett.getDbUser());
    m_con.setType("QPSQL");
    m_con.doConnect();
}

/** @brief Check the database version
  *
  * The function upgrade it if its version is below MainWindow's version.
  * @return true if the database is correctly installed and upgrade operations
  * goes fine.
  */
bool MainWindow::checkDB()
{
    simpleQuery query("MainWindow_db_version");

    QAbstractItemModel *model = query.getResult();

    if (likely(model != 0)) {
        QStringList actualVersion, lastVersion;
        lastVersion << "0" << "4" << "1";

        actualVersion << model->data(model->index(0,0)).toString();
        actualVersion << model->data(model->index(0,1)).toString();
        actualVersion << model->data(model->index(0,2)).toString();

        if (actualVersion != lastVersion) {
            delete model;
            return upgradeFrom(actualVersion, lastVersion);
        }
        return true;
    } else {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Error executing a query"));
        msgBox.setInformativeText(query.getErrorMessage());
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
    }

    return false;
}

bool MainWindow::upgradeFrom(const QStringList &actualVersion,
                            const QStringList &lastVersion)
{
    Q_UNUSED(actualVersion);
    Q_UNUSED(lastVersion);

    return true;
}

/** @brief Try to install the database with the default postgreSQL user
  *
  * @return true if the database is correctly installed
  * @see execSQLFile
  */
bool MainWindow::installDBFromZero()
{
    bool ok;
    QString username = QInputDialog::getText(0, tr("Default User name"),
                                         tr("User name:"), QLineEdit::Normal,
                                         "postgres", &ok);
    if (unlikely(!ok))
        return false;

    QString password = QInputDialog::getText(0, tr("Default Password"),
                                             tr("Password:"),
                                             QLineEdit::PasswordEchoOnEdit,
                                             "postgres", &ok);
    if (unlikely(!ok))
        return false;

    QString DBName = QInputDialog::getText(0, tr("Default Database"),
                                             tr("Name of the default database:"),
                                             QLineEdit::Normal,
                                             "postgres", &ok);
    if (unlikely(!ok))
        return false;

    m_con.setUser(username);
    m_con.setPassword(password);
    m_con.setDBName(DBName);

    try {
        if (unlikely(! m_con.canConnect()))
            return false;

        if ( ! m_con.doConnect() )
            return false;
    } catch (QString e) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Error when trying to connect to postgres database"));
        msgBox.setInformativeText(e);
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
        return false;
    }

    /** @todo This part is executed also when a user johnny already exists, and
      * the function returns without the creation of the database.
      */
    if ( ! execSQLFile("DatabaseCreation.sql") )
        return false;

    m_con.closeConnection();

    try {
        connectDefault();
    } catch (QString e) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Error when trying to connect to MainWindow database"));
        msgBox.setInformativeText(e);
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
        return false;
    }

    if (unlikely(! execSQLFile("DatabaseSchema.sql")))
        return false;

    if (unlikely(! execSQLFile("data_example_dump.sql")))
        return false;

    return true;
}

/** @brief Execute an entire file as SQL commands
  *
  * @return true if the file is found and if it executed correctly.
  */
bool MainWindow::execSQLFile(const QString& fileName) {
    QFile file(m_dbPath + "/" + fileName);
    QString line = QString::null;

    Q_ASSERT(file.exists());

    if ( likely(file.open(QIODevice::ReadOnly | QIODevice::Text)) ) {
        QTextStream t( &file );
        while (line += t.readLine(), !line.isNull()) {

            // Ignore line with comments (All lines starting with -)
            if (line.isEmpty() || line.at(0) == '-') {
                line = QString::null;
                continue;
            }

            // When there is a ; and we're not in a function, the line must be
            // executed.
            if (line.at(line.size() - 1 ) == ';') {
                // If the line contains two $ symbol, we're in a function and
                // we need to continue parsing. Otherwise, we can execute.
                if (line.count("$$") == 1)
                    continue;

                // Execute the line
                simplePlainQuery query(line);

                if (unlikely(!query.execute())) {
                    QMessageBox msgBox;
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setText(tr("Error executing a query"));
                    msgBox.setInformativeText(query.getErrorMessage());
                    msgBox.setStandardButtons(QMessageBox::Ok);

                    msgBox.exec();
                    file.close();
                    return false;
                }

                // After it is executed, we need to forget what line contains,
                // and start from top again.
                line = QString::null;
            }
        }
        file.close();

        return true;
    }

    return false;
}
