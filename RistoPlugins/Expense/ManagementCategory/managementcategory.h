/*
 *  Copyright (c) 2010 Natale Patriciello <kronat@tiscali.it>
 *
 *  This file is part of RistorNAT.
 *
 *   RistorNAT is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   RistorNAT is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RistorNAT.If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef managementCategory_H
#define managementCategory_H

#include "plugininterface.h"
#include "ui_managementcategory.h"

namespace Ui {
    class managementCategoryUi;
}

class managementCategory : public pluginInterface {
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    managementCategory(QWidget *parent = 0);
    ~managementCategory();

    /** @brief Name of the plugin
     *  @return Plugin name
     */
    virtual QString name() { return tr("Management\nCategory") ; }

    /** @brief Plugin's icon */
    QIcon icon() { return QIcon(":/journal.png");  }

    QString family() { return "Expense"; }

public slots:
    void okPressed();
    void askToSave();
    void showDetail(QModelIndex);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::managementCategoryUi ui;
    QStandardItemModel m_viewModel;
};

#endif // managementCategory_H
