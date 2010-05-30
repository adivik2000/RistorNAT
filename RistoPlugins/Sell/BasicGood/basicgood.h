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
#ifndef BASICGOOD_H
#define BASICGOOD_H

#include "plugininterface.h"

class simpleTable;

/** @brief Store and retrieve your basic good
  *
  * A basic good is a natural product that help you to cook various thing.
  * For example, tomatoes are basic goods.\n
  * @image html basic_good.png
  * Use the table for inserting, editing or removing basic goods.
  * In <b>Name</b> column, insert the name of the good, in <b>um</b> its unit
  * of measurement.
  * @par Toolbar
  * Pressing Refresh cost, you will refresh the average cost for the unit of
  * measurement selected.
  *
  */
class basicGood : public pluginInterface {
    Q_OBJECT
    Q_INTERFACES(pluginInterface)

public:
    /** @brief Constructor
      *
      */
    basicGood(QWidget *parent = 0);

    /** @brief Plugin's name
      * @return Translated plugin's name
      */
    virtual QString name() { return tr("Basic Good"); }

    /** @brief Plugin's icon
      */
    QIcon icon() { return QIcon(":/codice_a_barre48x48.png"); }

    /** @brief Plugin's family
      * @return Sell
      */
    QString family() { return "Sell"; }

private slots:
    void deleteFromStock();
    void refreshAvgCost();

private:
    simpleTable *m_table; /**< Main table */
};

#endif // BASICGOOD_H
