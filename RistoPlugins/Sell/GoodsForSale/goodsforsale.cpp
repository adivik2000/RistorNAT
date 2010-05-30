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
#include "goodsforsale.h"
#include <QtPlugin>
#include <QVBoxLayout>
#include <QMessageBox>
#include <advancedtable.h>
#include <simplequery.h>

/** @brief Constructor
  *
  * Remove the first column in the table good_for_sale
  * @param parent Parent of the plugin
  */
goodsForSale::goodsForSale(QWidget *parent):pluginInterface(parent)
{
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QAction *refresh = new QAction(QIcon(":/refresh.svg"),tr("Refresh Costs"),toolBar);
    connect(refresh,SIGNAL(triggered()),this,SLOT(updateCost()));
    toolBar->addAction(refresh);

    m_table = new advancedTable(this);
    m_table->setTableName("good_for_sale");
    m_table->setDefaultValue(0,QVariant(),true);
    m_table->addRelation(2,"goods_category","description","description");
    m_table->addRelation(3,"sell_category","description","description");
    m_table->addRelation(5,"unit_of_measurement","name","name");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(toolBar);
    layout->addWidget(m_table);
    setLayout(layout);
}

/** @brief Update the average cost for articles selected
  */
void goodsForSale::updateCost()
{
    simpleQuery query("set_article_average_cost");

    QItemSelectionModel *_selectionModel = m_table->selectionModel();
    QModelIndexList indexes = _selectionModel->selectedIndexes();
    QVector<int> map;

    foreach(QModelIndex index, indexes) {
        if (map.contains(index.row()))
            continue;

        map.append(index.row());
        QModelIndex idx = index.sibling(index.row(),1);
        paramList param;
        param.append(idx.data());
        query.setParameters(param);

        if (! query.execute()) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("Error executing a query"));
            msgBox.setInformativeText(tr("Query for cost of ") +
                                         index.data().toString() +
                                         tr(" failed."));
            msgBox.setStandardButtons(QMessageBox::Ok);

            msgBox.exec();
        }
    }
    m_table->setTableName("good_for_sale");

    if (indexes.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("No article(s) selected"));
        msgBox.setInformativeText(tr("Select one or more articles."));
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
    }
}

Q_EXPORT_PLUGIN2(goodsforsale, goodsForSale);
