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

#include <dbmanagement.h>

#include <QMessageBox>
#include <QPluginLoader>
#include <QDir>
#include <QFileDialog>
#include <QDebug>

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
    QCoreApplication::setApplicationName("RistorNAT");
    QCoreApplication::setApplicationVersion("0.5.0");

    ui->setupUi(this);
    setWindowTitle(QCoreApplication::applicationName()+ " " +
                   QCoreApplication::applicationVersion());

    QWidget *mainWidget;
    try {
        if (!setupDatabase())
            return;
        mainWidget = loadApp(ui->menuBar);
    } catch (QString e) {
        qDebug() << e;
    }

    Q_ASSERT(mainWidget != 0);

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
    disconnect(m_main,SIGNAL(currentChanged(int)),this,SLOT(pageChanged(int)));
    QMapIterator<QString,stackedWContainer*> i(m_fam);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }
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
QWidget* MainWindow::loadApp(QMenuBar* menuBar)
{
    Q_UNUSED(menuBar);
    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->setMovable(true);

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

             if (! m_fam.contains(family)) {
                stackedWContainer *cont = new stackedWContainer(tabWidget);
                m_numberFam.insert(tabWidget->addTab(cont,family),family);
                m_fam.insert(family,cont);
             }

             stackedWContainer *cont = m_fam.value(family);
             cont->add(child);

             currentPage = cont;
         } else {
             QMessageBox msgBox;
             msgBox.setIcon(QMessageBox::Warning);
             msgBox.setText(tr("Error loading a plugin"));
             msgBox.setInformativeText(pluginLoader.errorString());
             msgBox.setStandardButtons(QMessageBox::Ok);

             msgBox.exec();
         }
     }

    connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(pageChanged(int)));

    m_main = tabWidget;
    return tabWidget;
}

void MainWindow::dump()
{
    dbManagement ref;

    ref.dump("RistorNAT.sql");
}

/** @brief Slot for closing active plugin when the page is changed
  *
  * @param index Index of the new page
  */
void MainWindow::pageChanged(int index)
{
    Q_ASSERT(currentPage != 0);
    currentPage->aboutToBeClosed();

    currentPage = m_fam.value(m_numberFam.value(index),0);
    Q_ASSERT (currentPage != 0);

    currentPage->aboutToBeOpened();
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
    about->labelIcon->setPixmap(windowIcon().pixmap(128,128,QIcon::Normal,QIcon::On));
    about->labelIcon->setEnabled(true);

    box->exec();

    delete about;
    delete box;
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
    dbManagement ref;

    try {
        ref.connectDefault(m_con);
    } catch ( QString errorMessage ) {
        Q_UNUSED(errorMessage);
        // Ask for a new installation
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("Database doesn't exist"));
        msgBox.setInformativeText(tr("Do you want to try a new installation?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);

        int ret = msgBox.exec();

        if (likely(ret == QMessageBox::Yes)) {// New Installation
            QString dbfile = QFileDialog::getOpenFileName(0,
                                                        tr("Select the"
                                                           " database schema"));
            if (dbfile.isEmpty())
                return false;
            QString fnfile = QFileDialog::getOpenFileName(0,
                                                          tr("Select the"
                                                             " database functions"));
            if (fnfile.isEmpty())
                return false;

            QString dbcreat = QFileDialog::getOpenFileName(0,
                                                        tr("Select the creation"
                                                           " database file"));
            if (dbcreat.isEmpty())
                return false;

            return ref.installDBFromZero(m_con,dbfile,fnfile,dbcreat);
        } else
            return false;
    }

    return ref.checkDB("ristornat_db_version",QStringList()<<"0"<<"5"<<"0");
}
