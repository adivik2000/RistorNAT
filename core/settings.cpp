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
#include "settings.h"

/** @brief Default constructor
  * Initialize a new object QSettings, used to store parameters and values.
  * @see initializeDBSettings
  */
Settings::Settings()
{
    settingsFile = new QSettings;
    initializeDBSettings();
}

/** @brief Default De-Constructor
  * Delete the QSettings object created on the constructor
  * @see Settings
  */
Settings::~Settings()
{
    delete settingsFile;
}

/** @brief Control if it is the first time that Settings is created
  * If it is the first time that Settings is created, we need to store
  * default value of connection parameters.\n
  * Absolutely, these parameters can be modified by the user, and doesn't are
  * persistant. After the first run, a persistent file will rememenber
  * the settings.
  */
void Settings::initializeDBSettings()
{
    if ( ! settingsFile->contains("dbname")) {
        settingsFile->setValue("dbname","ristornat");
        settingsFile->setValue("username","johnny");
        settingsFile->setValue("password","johnny");
        settingsFile->setValue("host","localhost");
        settingsFile->setValue("port","5432");
    }
}

/** @brief Set persistent database name.
  * @param name Name of the Database
  */
void Settings::setDbName(const QString& name)
{
    settingsFile->setValue("dbname",name);
}

/** @brief Set persistent password.
  * @param pass Password
  */
void Settings::setDbPassword(const QString& pass)
{
    settingsFile->setValue("password",pass);
}

/** @brief Set persistent dbms username
  * @param user Username
  */
void Settings::setDbUser(const QString& user)
{
    settingsFile->setValue("username",user);
}

/** @brief Set persistent dbms host
  * @param host Host's address
  */
void Settings::setDbHost(const QString& host)
{
    settingsFile->setValue("host",host);
}

/** @brief Set persistent dbms port
  * @param port Port
  */
void Settings::setDbPort(const QString& port)
{
    settingsFile->setValue("port",port);
}

/** @brief Get persistent db name
  * @return Name of the database
  */
QString Settings::getDbName()
{
    return settingsFile->value("dbname").toString();
}

/** @brief Get persistent dbms password
  * @return DBMS password (unencrypted)
  */
QString Settings::getDbPassword()
{
    return settingsFile->value("password").toString();
}

/** @brief Get persistent dbms username
  * @return DBMS username
  */
QString Settings::getDbUser()
{
    return settingsFile->value("username").toString();
}

/** @brief Get persistent dbms host
  * @return Host's address
  */
QString Settings::getDbHost()
{
    return settingsFile->value("host").toString();
}

/** @brief Get persistent dbms port
  * @return DBMS port
  */
QString Settings::getDbPort()
{
    return settingsFile->value("port").toString();
}
