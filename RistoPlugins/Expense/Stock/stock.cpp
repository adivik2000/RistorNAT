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
#include "stock.h"
#include <QtPlugin>
#include <simplequery.h>
#include <QDebug>
/** @brief Constructor
  */
Stock::Stock(QWidget *parent):pluginInterface(parent)
{
    stockUi.setupUi(this);
    stockUi.tableActualStock->setTableName("stock");

    m_viewModel.insertColumn(0);
    m_viewModel.insertColumn(1);
    m_viewModel.insertColumn(2);
    m_viewModel.setHeaderData(0,Qt::Horizontal,tr("Article"));
    m_viewModel.setHeaderData(1,Qt::Horizontal,tr("Quantity"));
    m_viewModel.setHeaderData(2,Qt::Horizontal,tr("Unit of measurement"));

    stockUi.tableReportStock->setModel(&m_viewModel);
}

void Stock::okPressed()
{
    int rowDone = 0;
    QItemSelectionModel *_selectionModel = stockUi.tableActualStock->selectionModel();
    QModelIndexList indexes = _selectionModel->selectedRows();
    QModelIndex index;
    QStandardItemModel *modelView = qobject_cast<QStandardItemModel*>(
            stockUi.tableReportStock->model());
    Q_ASSERT(modelView != 0);

    modelView->removeRows(0,modelView->rowCount());

    foreach(index, indexes)
    {
        QVariant article = index.data();
        paramList param;
        param.append(article);
        param.append(stockUi.dateFrom->date());
        param.append(stockUi.dateTo->date());

        simpleQuery query("report_hist_stock", param);
        QAbstractItemModel *model = query.getResult();

        if (likely(model != 0)) {
            Q_ASSERT(model->rowCount() == 1);

            QStandardItem *art = new QStandardItem(article.toString());
            QStandardItem *qty = new QStandardItem(model->data(model->index(0,1)).toString());
            QStandardItem *um = new QStandardItem(model->data(model->index(0,2)).toString());

            if (qty->text().isEmpty()) {
                qty->setText("0");
            }

            modelView->setItem(rowDone, 0, art);
            modelView->setItem(rowDone, 1, qty);
            modelView->setItem(rowDone, 2, um);
        }
        rowDone++;
        delete model;
    }
}

Q_EXPORT_PLUGIN2(stock, Stock);
