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
#include "stock.h"
#include <QtPlugin>
#include <simplequery.h>

/** @brief Constructor
  */
Stock::Stock(QWidget *parent):pluginInterface(parent)
{
    stockUi.setupUi(this);
    stockUi.tableActualStock->setTableName("stock");
}

void Stock::okPressed()
{
    QVariant amount;
    simpleQuery query("report_hist_stock");
    paramList param;
    param.append(stockUi.dateFrom->date());
    param.append(stockUi.dateTo->date());

    query.setParameters(param);

    query.execute(amount);

    if (amount.isValid()) {
        stockUi.lcdNumber->display(amount.toDouble());
    }
}

Q_EXPORT_PLUGIN2(stock, Stock);
