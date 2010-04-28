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
#include "goodsforsale.h"
#include <QtPlugin>
#include <QVBoxLayout>
#include <advancedtable.h>

/** @brief Constructor
  *
  * Remove the first column in the table good_for_sale
  * @param parent Parent of the plugin
  */
goodsForSale::goodsForSale(QWidget *parent):pluginInterface(parent)
{
    advancedTable *table = new advancedTable(this);
    table->setTableName("good_for_sale");

    QVBoxLayout *layout = new QVBoxLayout(this);
    goodsDetail *det = new goodsDetail(this);

    layout->addWidget(table);
    layout->addWidget(det);

    setLayout(layout);

    table->setDefaultValue(0,QVariant(),true);
    table->addRelation(1,"goods_category","code","code");
    table->addRelation(2,"sell_category","code","code");
    table->addRelation(5,"unit_of_measurement","short_name","short_name");

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(table,SIGNAL(pressed(QModelIndex)),det,SLOT(updateInfo(QModelIndex)));
}

goodsForSale::~goodsForSale()
{
}

Q_EXPORT_PLUGIN2(goodsforsale, goodsForSale);
