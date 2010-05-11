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
#include <simpletable.h>

Stock::Stock(QWidget *parent):pluginInterface(parent)
{
    simpleTable *table = new simpleTable(parent);

    table->setTableName("stock");

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(table);

    setLayout(layout);
}

Q_EXPORT_PLUGIN2(stock, Stock);
