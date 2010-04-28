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
#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <QWidget>
#include <QMenuBar>
#include <QToolBar>

/** @brief Interface for application-plugins
  *
  * This is the main specific interface for _all_ application-plugins that
  * want to work under The NAT Management Software.\n
  * Every application-plugin need to have declared and implemented the
  * following methods, that do sequentially (no thread) operations to make
  * the application-plugin working.
  * @see setupUi
  * @see setupDatabase
  */
class appInterface
{
public:
    /** @brief Deconstructor for the application-plugin
      *
      * Virtual deconstructor that will contain the operations executed when
      * the application plugin is destroyed (like when it is unloaded).\n
      * The operations are, in most cases, freeying the memory allocated in
      * the plugin and closing the connection to database used by plugin.\n
      * Note that _all_ widget constructed in the application plugin need
      * to be removed from the workspace and deallocated from stack; otherwise,
      * there will be a memory leak, because the MainWindow doesn't know
      * how widgets need to be removed/destroyed.
      */
    virtual ~appInterface() {}

    /** @brief Setup the User Interface
      */
    virtual QWidget* setupUi(QMenuBar* menuBar, QToolBar *toolBar) = 0;

    /** @brief Set the Plugins search path
      *
      * Every application needs to know where it can find its plugins.\n
      * Remember to call before setupUi invocation.
      * @param path Path for the plugins
      */
    virtual void setPluginsPath(const QString& path) = 0;

    /** @brief Set the Icon search path
      *
      * Every application needs to know where it can find its icons.\n
      * Remember to call before setupUi invocation.
      * @param path Path for the icons
      */
    virtual void setIconPath(const QString& path) = 0;

    /** @brief Set the Database files search path
      *
      * Every application needs to know where it can find some database's file
      * (like database schema, or sample data).\n
      * Remember to call before setupDatabase invocation.
      * @param path Path for the files for the database
      */
    virtual void setDbPath(const QString& path) = 0;

    /** @brief Setup the database connection
      *
      * setupDatabase do all the things useful for the database, like open
      * a connection, install an example database.. and much more.
      */
    virtual bool setupDatabase() = 0;
};

 Q_DECLARE_INTERFACE(appInterface,
                     "com.yourcompany.Plugin.appInterface/1.0");
#endif // APPINTERFACE_H
