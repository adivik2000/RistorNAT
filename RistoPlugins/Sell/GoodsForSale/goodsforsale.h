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
#ifndef GOODSFORSALE_H
#define GOODSFORSALE_H

#include "plugininterface.h"

class advancedTable;

/** @brief Store and retrieve your selling articles
  *
  * With this interface, you could easily insert and modify selling articles,
  * like "Pasta al ragu".\n
  * @image html good_for_sale.png
  * Right-clicking the table, you could edit information about the selling
  * article.\n
  * You could insert the <b>Description</b>, a brief name of the article,
  * <b>Good Category</b>, the good category of the article, <b>Sell Category</b>,
  * the sell category of the article, <b>Quantity</b> is the standard quantity
  * of the article you will sell (statistical purpose), <b>um</b> unit of
  * measurement for the article, and <b>Last cost</b> and <b>Average Cost</b>
  * which are calculated by the program. You could safely insert 0 here.
  * @par Toolbar
  * By pressing Refresh Costs, the program recalculate the average cost of
  * the article selected.This operation could take much time, so be
  * patient.
  */
class goodsForSale : public pluginInterface
{
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    goodsForSale(QWidget *parent = 0);

    /** @brief Plugin's name
      * @return Translated plugin's name
      */
    virtual QString name() { return tr("Goods\nfor sale"); }

    /** @brief Plugin's icon */
    QIcon icon() { return QIcon(":/vendita48x48.png"); }

    /** @brief Plugin's family
      * @return Sell
      */
    QString family() { return "Sell"; }

private slots:
    void updateCost();

private:
    advancedTable *m_table; /**< Main Table */
};

#endif // GOODSFORSALE_H
