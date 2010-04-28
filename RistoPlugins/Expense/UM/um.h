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
#ifndef UM_H
#define UM_H

#include <plugininterface.h>

class simpleTable;
class advancedTable;
class QGridLayout;

/** @brief Store unit of measurement [Expense set]
  *
  */
class um : public pluginInterface {
    Q_OBJECT
    Q_INTERFACES(pluginInterface)

public:
    um(QWidget *parent = 0);

    ~um();

    /** @brief Name of the plugin
      * @return Translated plugin name
      */
    QString name() { return tr("Units of\nmeasurement"); }

    /** @brief Plugin's icon */
    QIcon icon() { return QIcon(":/categoria_fornitore48x48.png"); }

    QString family() { return "Expense"; }
};

#endif // UM_H
