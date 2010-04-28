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
#include "detail.h"

/** @brief Store and retrieve your selling articles [Sell set]
  *
  * @par Dependencies
  *
  * This plugin needs:
  * - goodsCategory
  * - sellCategory
  *
  * @par Description
  *
  * With this interface, you could easily insert and modify selling articles,
  * like "Pasta al ragu".\n
  * You could enable Detailed section, where you can see Last Cost, and
  * the Average Cost for the article selected. See Detail for more information.
  * @image html goods_for_sale.tiff
  * For the editing, you have this window, called from the right click menu:
  * @image html goods_for_sale_editing.tiff
  * - In the column <b>goods_category</b> insert the category for the article;
  * - In the column <b>sell_category</b> insert the selling category for the
  * article;
  * - In the column <b>description</b> insert the name of the article.
  */
class goodsForSale : public pluginInterface
{
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    goodsForSale(QWidget *parent = 0);
    ~goodsForSale();

    /** @brief Plugin's name
      * @return Translated plugin's name
      */
    virtual QString name() { return tr("Goods\nfor sale"); }

    QIcon icon() { return QIcon(":/vendita48x48.png"); }
    QString family() { return "Sell"; }
};

#endif // GOODSFORSALE_H
