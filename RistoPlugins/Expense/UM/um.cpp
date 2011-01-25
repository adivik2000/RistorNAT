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
#include <QtPlugin>

/** @brief Constructor
  *
  */
UnitOfMeasurement::UnitOfMeasurement(QWidget *parent):pluginInterface(parent)
{
    simpleTable *m_table = new simpleTable(this);
    QGridLayout *m_layout = new QGridLayout(this);

    m_table->setTableName("unit_of_measurement");
    m_table->addRelation(1,"unit_of_measurement","um","name");

    QStringList header;
    header << tr("Name") << tr("Base UM") << tr("Multiplicator");
    m_table->setHeader(header);

    QToolBar *toolBar = m_table->getToolBar();
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_layout->addWidget(toolBar,0,0);
    m_layout->addWidget(m_table,1,0);

    setLayout(m_layout);
}

Q_EXPORT_PLUGIN2(UnitOfMeasurement, UnitOfMeasurement);
