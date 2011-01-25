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
#include <QMessageBox>

#include <simpletable.h>
#include <simplequery.h>
#include "sellcategory.h"

/** @brief Constructor
  *
  * Make also the model for report
  */
sellCategory::sellCategory(QWidget *parent):pluginInterface(parent)
{
    m_table = new simpleTable(this);
    m_table->setTableName("sell_category");

    QStringList header;
    header << tr("Name");
    m_table->setHeader(header);

    QGroupBox *detailBox = new QGroupBox(this);
    m_detail = new Ui::sellCategoryDetail;
    m_detail->setupUi(detailBox);
    connect(m_detail->buttonOk,SIGNAL(clicked()),this,SLOT(updateReport()));

    QToolBar *toolBar = m_table->getToolBar();
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(toolBar);
    layout->addWidget(m_table);
    layout->addWidget(detailBox);

    QStandardItemModel *viewModel = new QStandardItemModel(m_detail->tableView);
    viewModel->insertColumn(0);
    viewModel->insertColumn(1);
    viewModel->setHeaderData(0,Qt::Horizontal,tr("Category"));
    viewModel->setHeaderData(1,Qt::Horizontal,tr("Amount"));

    m_detail->tableView->setModel(viewModel);

    setLayout(layout);
}

/** @brief Update the report for each category selected
  *
  */
void sellCategory::updateReport()
{
    int rowDone = 0;
    QItemSelectionModel *_selectionModel = m_table->selectionModel();
    QModelIndexList indexes = _selectionModel->selectedRows();
    QModelIndex index;
    QStandardItemModel *modelView = qobject_cast<QStandardItemModel*>(
            m_detail->tableView->model());
    Q_ASSERT(modelView != 0);

    modelView->removeRows(0,modelView->rowCount());

    foreach(index, indexes)
    {
        QVariant category = index.data();

        paramList param;
        param.append(category);
        param.append(QVariant(m_detail->dateFrom->date()));
        param.append(QVariant(m_detail->dateTo->date()));

        simpleQuery query("report_sellcategory_amount",param);

        QAbstractItemModel *model = query.getResult();
        QVariant amount;

        if (likely (model != 0 && model->rowCount() == 1)) {
            amount = model->data(model->index(0,0));

            if (unlikely(amount.toString().isEmpty())) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(tr("No information for category"));
                msgBox.setInformativeText(tr("Category ") + category.toString() +
                                          tr(" has no financial information."));
                msgBox.setStandardButtons(QMessageBox::Ok);

                msgBox.exec();
                continue;
            }

            QStandardItem *item = new QStandardItem(category.toString());
            QStandardItem *item2 = new QStandardItem(amount.toString()+" $");

            modelView->setItem(rowDone,0,item);
            modelView->setItem(rowDone,1,item2);

            rowDone++;
            delete model;
        }
    }
}

Q_EXPORT_PLUGIN2(sellcategory, sellCategory);
