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

/** @brief Namespace for UI classes */
namespace Ui {
    class managementCategoryUi;
}

/** @brief Manage management category
  *
  * For each category, like Entertainment (where you'll place all expense
  * for entertainment), you could retrieve the amount, selecting the period.
  * @image html management_category.png
  * In the first table, you could insert, edit or delete category, by
  * right-clicking into the table. Edit is also possible, by double-clicking on
  * the item you want to modify.\n
  * If you select one or more category, and the period (using the two date editor
  * date from and date to), by pressing Ok you'll have a report, for each
  * category selected, of its total amount. If you click on one row, you'll
  * have detailed report, with each documents which have contribuited to the
  * amount.
  */
class managementCategory : public pluginInterface {
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    managementCategory(QWidget *parent = 0);
    ~managementCategory();

    /** @brief Name of the plugin
     *  @return Name: Management Category
     */
    virtual QString name() { return tr("Management\nCategory") ; }

    /** @brief Plugin's icon */
    QIcon icon() { return QIcon(":/journal.png");  }

    /** @brief Plugin's family
      * @return Family "Expense"
      */
    QString family() { return "Expense"; }

public slots:
    void okPressed();
    void askToSave();
    void showCategoryDetail(QModelIndex);
    void categoryClicked();
    void descriptionClicked();

protected:
    void changeEvent(QEvent *e);
    void report(const QTableView& table, const QString& fn);

private:
    Ui::managementCategoryUi ui; /** @brief User interface object */
    QStandardItemModel m_viewModel; /** @brief Model for the detailed view */
    bool m_table_active[2];
};

#endif // managementCategory_H
