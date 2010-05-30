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
#include "updateamountdelegate.h"
#include "ui_managementcost.h"
#include "plugininterface.h"

namespace Ui {
    class managementCostUi;
}

/** @brief User interface for ManagementCost
  *
  * @image html management_cost.png
  *
  * @par Header
  * In the header section, you must insert the header of the invoice, with
  * the classical seller selection (use seller plugin to insert/edit/remove sellers),
  * the date and the reference number.\n
  * You <b>must</b> select if you want to insert Food & Beverage cost or if you
  * want to insert a management cost, for statistical purpose.
  *
  * @par Row
  * Depending of what kind of cost you've typed (F&B or purchasing) the table
  * row will be populated. The column are:
  * -# For Food&Beverage:\n
  * <b>Good</b> (good you have purchased, use basicGood to insert/edit/remove goods \n
  * <b>Quantity</b> the quantity you have purchased \n
  * <b>UM</b> Indicates the unit of measure for the good \n
  * <b>Single price</b> Indicates the price for 1 (one) unit (which you have
  *  previously selected).
  * -# For Purchasing costs:\n
  * <b>Category</b> for the category of the cost
  * <b>Price</b> the price of the cost.
  *
  * @par Toolbar
  * The commands in toolbar are:\n
  * <b>Ok</b> Save all the information, Header and Row, in the database and
  * clean the interface.\n
  * <b>Clean</b> Clean the interface without saving.\n
  * <b>Delete</b> Delete the documents and its row from the database.\n
  * <b>List Document</b> Get a list of document. Remember to select the type
  * before list documents.
  */
class managementCost : public pluginInterface {
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    managementCost(QWidget *parent = 0);
    ~managementCost();

    /** @brief Name of the plugin
     *  @return Plugin name, Management Cost
     */
    virtual QString name() { return tr("Management\nCost") ; }

    /** @brief Plugin's icon
      * @return icon for cassa48x48.png
      */
    QIcon icon() { return QIcon(":/cassa48x48.png");  }

    /** @brief Plugin's family
      * @return Expense
      */
    QString family() { return "Expense"; }

    void aboutToBeOpened();

public slots:
    void okPressed();
    void deletePressed();
    void undoPressed();
    void listPressed();
    void goPressed();
    void saveAmountDisplayed();
    void delFromAmountDisplayed();
    void addToAmountDisplayed();

protected:
    void changeEvent(QEvent *e);
    QVariant populateTable(const QString& functionName,
                           const QString& rowTableName,
                           const QString& amountFnName);
    void enableHeader(bool enable);
    double getAmountForRow(QModelIndex idx);

private:
    Ui::managementCostUi ui; /**< @brief UserInterface */
    QVariant m_documentId; /**< @brief Active document id */
    comboBoxDelegate *m_comboCategory; /** @brief Delegate for category */
    comboBoxDelegate *m_comboUM; /** @brief Delegate for UM */
    comboBoxDelegate *m_comboGood; /** @brief Delegate for goods */
    updateAmountPurchDelegate *m_purchPriceDel; /** @brief Delegate for
                                                  * updating the amount in
                                                  * purchasing cost*/
    updateAmountGoodsDelegate *m_goodsDel; /** @brief Delegate for updating
                                             * the amount in goods cost */
};

#endif // COSTUI_H
