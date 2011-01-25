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
#include <QToolBar>
#include <QAction>

/** @brief Constructor
  *
  * @param parent Parent
  */
basicGood::basicGood(QWidget *parent):pluginInterface(parent)
{
    m_table = new simpleTable(this);
    m_table->setTableName("basic_good");
    m_table->setColumnWidth(0,300);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->addRelation(1,"unit_of_measurement","name","name");

    QStringList header;
    header << tr("Name") << tr("UM") << tr("Average cost");
    m_table->setHeader(header);

    QToolBar *toolBar = m_table->getToolBar();
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QAction *m_refresh = new QAction(QIcon(":/refresh.svg"),
                                     tr("Refresh Costs"),toolBar);
    toolBar->addSeparator();
    toolBar->addAction(m_refresh);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(toolBar);
    layout->addWidget(m_table);
    setLayout(layout);

    connect(m_table,SIGNAL(beforeDelete()),this,SLOT(deleteFromStock()));
    connect(m_refresh,SIGNAL(triggered()),this,SLOT(refreshAvgCost()));
}

/** @brief Refresh the average cost for goods selected
  */
void basicGood::refreshAvgCost()
{
    /** @todo date are always null */
    simpleQuery query("set_good_average_cost");

    QItemSelectionModel *_selectionModel = m_table->selectionModel();
    QModelIndexList indexes = _selectionModel->selectedRows();

    foreach(QModelIndex index, indexes) {
        QModelIndex idx = index.sibling(index.row(),0);
        paramList param;
        param.append(idx.data());
        query.setParameters(param);

        if (! query.execute()) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("Error executing a query for ") + idx.data().toString());
            msgBox.setInformativeText(query.getErrorMessage());
            msgBox.setStandardButtons(QMessageBox::Ok);

            msgBox.exec();
        }
    }
    m_table->setTableName("basic_good");

    if (indexes.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("No goods selected! "));
        msgBox.setInformativeText("Select one or more goods from list below.");
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
    }
}

/** @brief Delete goods from stock
  */
void basicGood::deleteFromStock()
{
    simpleQuery query;
    paramList param;

    QItemSelectionModel *_selectionModel = m_table->selectionModel();
    QModelIndexList indexes = _selectionModel->selectedRows();

    foreach(QModelIndex index, indexes) {
        query.setFunctionName("check_good_in_doc");
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
