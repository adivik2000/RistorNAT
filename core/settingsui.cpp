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
#include "settingsui.h"
#include "ui_settingsui.h"

/** @brief Default
  * Initialize the Graphic User Interface, and creates new Settings object.
  * @see Settings
  */
settingsUi::settingsUi(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::settingsUi)
{
    m_ui->setupUi(this);
    settings = new Settings();
}

/** @brief Default deconstructor
  * Deallocate the memory for the gui and for the Settings object.
  */
settingsUi::~settingsUi()
{
    delete m_ui;
    delete settings;
}

/**
  * Retranslate the gui
  */
void settingsUi::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/** @brief Catch the OK button pression
  * @see saveDBSettings
  */
void settingsUi::okPressed()
{
    saveDBSettings();
    this->close();
}

/** @brief Close the widget
  *
  * Close and destroy widget when cancel button is pressed.
  */
void settingsUi::cancelPressed()
{
    this->close();
}

/** @brief Store database values for default parameters
  *
  * Store permanently, using a Setting object, the database default
  * information that the user has modified.
  * @see Settings
  */
void settingsUi::saveDBSettings()
{
    settings->setDbName(m_ui->dbname_text->text());
    settings->setDbPassword(m_ui->password_text->text());
    settings->setDbUser(m_ui->username_text->text());
    settings->setDbHost(m_ui->host_text->text());
    settings->setDbPort(m_ui->port_text->text());
}
