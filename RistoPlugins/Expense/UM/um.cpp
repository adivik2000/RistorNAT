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
#include "um.h"
#include <advancedtable.h>
#include <workdelegate.h>

#include <QLabel>
#include <QtPlugin>
#include <QGridLayout>

/** @brief Constructor
  *
  */
um::um(QWidget *parent):pluginInterface(parent)
{
    simpleTable *tableComp = new simpleTable(this);
    comboBoxDelegate *comboUM = new comboBoxDelegate("unit_of_measurement",0,0,this);

    tableComp->setTableName("unit_of_measurement");
    tableComp->addRelation(2,"unit_of_measurement","base","name");
    tableComp->setItemDelegateForColumn(1,comboUM);

    QLabel *desc_adv = new QLabel(this);
    desc_adv->setText(tr("Insert the unit of measurement which will be used"
                         " by other plugins, like Kg, g, lt, cl.. and don't"
                         " worry if you see duplicates."));

    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(desc_adv,0,0);
    layout->addWidget(tableComp,1,0);
}

/** @brief Deconstructor
  *
  */
um::~um()
{ }

Q_EXPORT_PLUGIN2(um, um);
