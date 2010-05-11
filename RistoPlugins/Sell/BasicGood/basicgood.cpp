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
#include <QtPlugin>
#include "basicgood.h"
#include "simpletable.h"

#include <simplequery.h>
#include <QMessageBox>

basicGood::basicGood(QWidget *parent):pluginInterface(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    m_table = new simpleTable(this);
    m_table->setTableName("basic_good");
    m_table->setColumnWidth(0,300);

    layout->addWidget(m_table);
    setLayout(layout);

    connect(m_table,SIGNAL(beforeDelete()),this,SLOT(deleteFromStock()));
}

void basicGood::deleteFromStock()
{
    simpleQuery query("check_good_in_doc");
    paramList param;

    QItemSelectionModel *_selectionModel = m_table->selectionModel();
    QModelIndexList indexes = _selectionModel->selectedRows();

    foreach(QModelIndex index, indexes) {
        param.append(index.data());
        query.setParameters(param);

        if (! query.execute()) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("Can't delete"));
            msgBox.setInformativeText(index.data().toString()+tr(
                    " has entries in document for goods.")+tr("\nRestore with Undo"));
            msgBox.setStandardButtons(QMessageBox::Ok);

            msgBox.exec();
            continue;
        }
        query.setFunctionName("delete_from_stock");
        query.execute();
    }
}

Q_EXPORT_PLUGIN2(basicgood, basicGood);
