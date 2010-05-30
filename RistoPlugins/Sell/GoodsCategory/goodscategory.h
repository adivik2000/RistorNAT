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
#ifndef GOODSCATEGORY_H
#define GOODSCATEGORY_H

#include <plugininterface.h>
#include <QtPlugin>

#include "ui_goodscategorydetail.h"
class simpleTable;

/** @brief Manage the article's categories
  *
  * With this plugin you can easily manage the categories of articles.
  * @image html goods_category.png
  * You could insert, edit or remove categories by right-clicking into the
  * first table.
  *
  * @par Report
  * For each category selected, you could generate a report by clicking into
  * Ok button. Be sure to select the period you want with the two date editor.\n
  * For each category will be reported the selled amount.
  *
  */
class goodsCategory : public pluginInterface {
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    /** @brief Constructors
      *
      */
    goodsCategory(QWidget *parent = 0);

    /** @brief Plugin name
      * @return Translated plugin name
      */
    QString name() { return tr("Goods Category"); }

    /** @brief Plugin's icon */
    QIcon icon() { return QIcon(":/categoria_articolo48x48.png"); }

    /** @brief Plugin's family
      * @return Sell
      */
    QString family() { return tr("Sell"); }

public slots:
    void updateReport();

private:
    Ui::goodsCategoryDetail *m_detail; /**< @brief Detail */
    simpleTable *m_table; /**< @brief Main table */
};

#endif // GOODSCATEGORY_H
