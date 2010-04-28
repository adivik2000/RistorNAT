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
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

namespace Ui {
    class Settings;
}

/** @brief Class to store permanenty settings and other information
  *
  * This class, using a QSettings object, store database information.
  * The information are saved in a os-dependent way (XML on OsX).
  * @see Settings
  * @see initializeDBSettings
  */
class Settings {
public:
    Settings();
    ~Settings();
    void setDbName(const QString&);
    void setDbUser(const QString&);
    void setDbPassword(const QString&);
    void setDbHost(const QString&);
    void setDbPort(const QString&);
    QString getDbName();
    QString getDbUser();
    QString getDbPassword();
    QString getDbHost();
    QString getDbPort();

private:
    QSettings *settingsFile; /**< Object that store/load settings */

    void initializeDBSettings();
};

#endif // SETTINGS_H
