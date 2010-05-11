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

/** @brief Store and retrieve your basic good [Sell set]
  *
  * A basic good is a natural product that help you to cook various thing.
  * For example, tomatoes are a basic good.\n
  * For store and retrieve your basic good, you can use this interface:
  * @image html basic_good.tiff
  * - In the <b>code</b> column insert a brief code, like COD001
  * - In the <b>description</b> column insert the name of the basic good.
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

    QIcon icon() { return QIcon(":/codice_a_barre48x48.png"); }

    QString family() { return "Sell"; }

public slots:
    void deleteFromStock();

private:
    simpleTable *m_table;
};

#endif // BASICGOOD_H
