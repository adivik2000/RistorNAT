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
#ifndef COSTUI_H
#define COSTUI_H

#include <QList>
#include <QAction>
#include <workdelegate.h>
#include "ui_managementcost.h"
#include "plugininterface.h"

namespace Ui {
    class managementCostUi;
}

/** @brief User interface for ManagementCost
  *
  * @image html ManagementCost.tiff
  *
  * On the top, you could check if you're inserting a Purchasing Cost
  * or a Good Cost. After you choice, you can select the Seller from the
  * combobox, and then insert the date and the number.\n
  * After you insert the number, the table view is populated if there already
  * exist a previous document, or a blank model if it is empty.\n
  * You could now insert / edit / delete row as the usual way.\n
  * On the top, you see three buttons: <b>Ok</b> is for saving, <b>Delete</b>
  * is for deleting document an the row linked in, and <b>Cancel</b> discard
  * the actual modifications.
  */
class managementCost : public pluginInterface {
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    managementCost(QWidget *parent = 0);
    ~managementCost();

    void setIconPath(const QString& iconPath);

    /** @brief Name of the plugin
     *  @return Plugin name
     */
    virtual QString name() { return tr("Management\nCost") ; }

    /** @brief Plugin's icon */
    QIcon icon() { return QIcon(":/cassa48x48.png");  }

    QString family() { return "Expense"; }

    void aboutToBeOpened();

public slots:
    void okPressed();
    void deletePressed();
    void undoPressed();
    void listPressed();
    void goPressed();
    void updateAmount();

protected:
    void changeEvent(QEvent *e);
    QVariant populateTable(const QString& functionName,
                           const QString& rowTableName,
                           const QString& amountFnName);
    void enableHeader(bool enable);

private:
    Ui::managementCostUi ui; /**< @brief UserInterface */
    QVariant m_documentId; /**< @brief Active document id */
    comboBoxDelegate *m_comboCategory;
    comboBoxDelegate *m_comboUM;
    comboBoxDelegate *m_comboGood;
};

#endif // COSTUI_H
