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
#ifndef STOCK_H
#define STOCK_H

#include "plugininterface.h"

class Stock : public pluginInterface
{
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    Stock(QWidget *parent = 0);

    /** @brief Name of the plugin
     *  @return Plugin name
     */
    virtual QString name() { return tr("Stock") ; }

    /** @brief Plugin's icon */
    QIcon icon() { return QIcon(":/fornitore48x48.png");  }

    QString family() { return "Expense"; }
};

#endif // STOCK_H
