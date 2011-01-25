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
#include "seller.h"

#include <simplequery.h>
#include <QMessageBox>
#include <QtPlugin>

/** @brief Constructor
  *
  * Create also the model for the report.
  */
seller::seller(QWidget *parent) : pluginInterface(parent)
{
    ui.setupUi(this);
    ui.tableSeller->setTableName("supplier");

    QStringList header;
    header << tr("Name");
    ui.tableSeller->setHeader(header);

    QToolBar *toolBar = ui.tableSeller->getToolBar();
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui.toolLayout->addWidget(toolBar);

    m_viewModel.insertColumn(0);
    m_viewModel.insertColumn(1);
    m_viewModel.setHeaderData(0,Qt::Horizontal,tr("Seller"));
    m_viewModel.setHeaderData(1,Qt::Horizontal,tr("Amount"));

    ui.tableView->setModel(&m_viewModel);
}

/** @brief Deconstructor
  */
seller::~seller()
{
}

/** @brief Retranslate UI
  */
void seller::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

/** @brief Make the report for each sellers selected
  */
void seller::okPressed()
{
    int rowDone = 0;
    QItemSelectionModel *_selectionModel = ui.tableSeller->selectionModel();
    QModelIndexList indexes = _selectionModel->selectedRows();
    QModelIndex index;
    QStandardItemModel *modelView = qobject_cast<QStandardItemModel*>(
            ui.tableView->model());
    Q_ASSERT(modelView != 0);

    modelView->removeRows(0,modelView->rowCount());

    foreach(index, indexes)
    {
        QVariant seller = index.data();

        paramList param;
        param.append(seller);
        param.append(QVariant(ui.dateFrom->date()));
        param.append(QVariant(ui.dateTo->date()));

        simpleQuery query("report_seller_amount",param);

        QAbstractItemModel *model = query.getResult();
        QVariant amount;

        if (likely (model != 0 && model->rowCount() == 1)) {
            amount = model->data(model->index(0,0));

            if (unlikely(amount.toString().isEmpty())) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(tr("No information for supplier"));
                msgBox.setInformativeText(tr("Supplier ") + seller.toString() +
                                          tr(" has no financial information."));
                msgBox.setStandardButtons(QMessageBox::Ok);

                msgBox.exec();
                continue;
            }

            QStandardItem *item = new QStandardItem(seller.toString());
            QStandardItem *item2 = new QStandardItem(amount.toString()+" $");

            modelView->setItem(rowDone,0,item);
            modelView->setItem(rowDone,1,item2);

            rowDone++;
            delete model;
        }
    }
}

/** @brief Ask to save information before leaving the plugin
  */
void seller::askToSave()
{
    if (ui.tableSeller->isDirty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("The table has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                                  QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        int ret = msgBox.exec();

        if (ret == QMessageBox::Save)
                ui.tableSeller->saveChanges();
    }
}

Q_EXPORT_PLUGIN2(seller, seller);
