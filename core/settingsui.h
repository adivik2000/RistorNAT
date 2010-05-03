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
#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <QtGui/QDialog>
#include <settings.h>

namespace Ui {
    class settingsUi;
}

/** @brief Dialog box for inserting database's parameters
  *
  * This class shows a dialog box that can permit users to modify the
  * default parameters for connecting the database.
  * @see accept
  * @see reject
  * @see saveDBSettings
  */
class settingsUi : public QDialog {
    Q_OBJECT
public:
    settingsUi(QWidget *parent = 0);
    ~settingsUi();

protected:
    void changeEvent(QEvent *e);

public slots:
    void okPressed();
    void cancelPressed();

private:
    Ui::settingsUi *m_ui; /**< User Interface for the class */
    Settings *settings; /**< Store/Load the settings @see Settings*/
    void saveDBSettings();
};

#endif // SETTINGSUI_H
