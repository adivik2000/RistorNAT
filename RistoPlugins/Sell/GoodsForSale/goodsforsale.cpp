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
#include <QStringList>
#include <advancedtable.h>
#include <simplequery.h>

/** @brief Constructor
  *
  * Remove the first column in the table good_for_sale
  * @param parent Parent of the plugin
  */
goodsForSale::goodsForSale(QWidget *parent):pluginInterface(parent)
{
    m_ui.setupUi(this);
    m_ui.tableView->setTableName("good_for_sale");
    m_ui.tableView->setDefaultValue(0,QVariant(),true);
    m_ui.tableView->addRelation(2,"goods_category","description","description");
    m_ui.tableView->addRelation(3,"sell_category","description","description");
    m_ui.tableView->addRelation(5,"unit_of_measurement","name","name");
    m_ui.tableView->setColumnHidden(6, true);
    m_ui.tableView->setColumnHidden(7, true);

    QStringList header;
    header << "id" << tr("Article name") << tr("Good category")
           << tr("Sell category") << tr("Quantity") << tr("Unit of measurement")
           << tr("Last price") << tr("Average cost");
    m_ui.tableView->setHeader(header);

    QToolBar *toolBar = m_ui.tableView->getToolBar();
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QAction *refresh = new QAction(QIcon(":/refresh.svg"),tr("Refresh Costs"),toolBar);
    connect(refresh,SIGNAL(triggered()),this,SLOT(updateCost()));
    connect(m_ui.tableView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(refreshCostUi()));
    toolBar->addSeparator();
    toolBar->addAction(refresh);

    m_ui.toolLayout->addWidget(toolBar);
}

/** @brief Update the average cost for articles selected
  */
void goodsForSale::updateCost()
{
    simpleQuery query("set_article_average_cost");

    QItemSelectionModel *_selectionModel = m_ui.tableView->selectionModel();
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

    m_ui.tableView->refresh();

    if (indexes.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("No article(s) selected"));
        msgBox.setInformativeText(tr("Select one or more articles."));
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
    }
}

/** @brief Refresh statistics about selected good
  *
  */
void goodsForSale::refreshCostUi()
{
    QItemSelectionModel *_selectionModel = m_ui.tableView->selectionModel();
    QModelIndex index = _selectionModel->currentIndex();

    QModelIndex last_price = index.sibling(index.row(), 6);
    QModelIndex avg_cost = index.sibling(index.row(), 7);

    QVariant lastPrice = m_ui.tableView->model()->data(last_price);
    QVariant avgCost = m_ui.tableView->model()->data(avg_cost);

    // x=V-C is earn on this item;  x*100/V is the %
    QVariant earn = ((lastPrice.toDouble() - avgCost.toDouble()) * 100) / lastPrice.toDouble();
    QString earn_display;

    if (earn.toString() == "nan" || earn.toString() == "-inf" || earn.toString() == "inf")
        earn_display = tr("Can't calculate");
    else
        earn_display = earn.toString();

    m_ui.lastPrice_edit->setText(lastPrice.toString());
    m_ui.avgPrice_edit->setText(avgCost.toString());
    m_ui.guagagno_edit->setText(earn_display);

}

Q_EXPORT_PLUGIN2(goodsforsale, goodsForSale);
