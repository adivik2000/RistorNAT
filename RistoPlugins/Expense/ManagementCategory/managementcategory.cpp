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
#include "managementcategory.h"

#include <simplequery.h>
#include <QMessageBox>
#include <QtPlugin>

managementCategory::managementCategory(QWidget *parent) : pluginInterface(parent)
{
    ui.setupUi(this);

    ui.tableCategory->setTableName("cost_category");

    m_viewModel.insertColumn(0);
    m_viewModel.insertColumn(1);
    m_viewModel.setHeaderData(0,Qt::Horizontal,tr("Category"));
    m_viewModel.setHeaderData(1,Qt::Horizontal,tr("Amount"));

    ui.tableView->setModel(&m_viewModel);

    QStandardItemModel *model = new QStandardItemModel(this);
    ui.tableViewDetailed->setModel(model);

    connect(ui.tableView,SIGNAL(pressed(QModelIndex)),this,SLOT(showDetail(QModelIndex)));
}

managementCategory::~managementCategory()
{
}

void managementCategory::changeEvent(QEvent *e)
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

void managementCategory::okPressed()
{
    int rowDone = 0;
    QItemSelectionModel *_selectionModel = ui.tableCategory->selectionModel();
    QModelIndexList indexes = _selectionModel->selectedRows();
    QModelIndex index;
    QStandardItemModel *modelView = qobject_cast<QStandardItemModel*>(
            ui.tableView->model());
    QAbstractItemModel *modelDetailed = ui.tableViewDetailed->model();

    modelView->removeRows(0,modelView->rowCount());
    modelDetailed->removeRows(0,modelDetailed->rowCount());

    foreach(index, indexes)
    {
        QVariant category = index.data();

        paramList param;
        param.append(QVariant(ui.dateFrom->date()));
        param.append(QVariant(ui.dateTo->date()));
        param.append(category);

        simpleQuery query("report_management_category",param);

        QAbstractItemModel *model = query.getResult();

        if (likely (model != 0 && model->rowCount() == 1)) {
            QVariant amount = model->data(model->index(0,0));
            if (amount.toInt() == 0) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(tr("No information for the category"));
                msgBox.setInformativeText(tr("There aren't information for ")
                                             + category.toString());
                msgBox.setStandardButtons(QMessageBox::Ok);

                msgBox.exec();
                continue;
            }
            QStandardItem *item = new QStandardItem(category.toString());
            QStandardItem *item2 = new QStandardItem(model->data(
                    model->index(0,0)).toString());

            modelView->setItem(rowDone,0,item);
            modelView->setItem(rowDone,1,item2);

            rowDone++;
            delete model;
        }
    }
}

void managementCategory::showDetail(QModelIndex index)
{
    QVariant category;
    category = index.sibling(index.row(),0).data();

    paramList param;
    param.append(QVariant(ui.dateFrom->date()));
    param.append(QVariant(ui.dateTo->date()));
    param.append(category);

    simpleQuery query("report_management_category_detailed",param);
    QAbstractItemModel *model = query.getResult();

    if (likely(model != 0)) {
        QAbstractItemModel *old_model = ui.tableViewDetailed->model();
        model->setParent(ui.tableViewDetailed);
        ui.tableViewDetailed->setModel(model);
        delete old_model;
    } else {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Error executing a query"));
        msgBox.setInformativeText(query.getErrorMessage());
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
    }
}

void managementCategory::askToSave()
{
    if (ui.tableCategory->isDirty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("The table has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                                  QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        int ret = msgBox.exec();

        if (ret == QMessageBox::Save)
                ui.tableCategory->saveChanges();
    }
}

Q_EXPORT_PLUGIN2(managementcategory, managementCategory);
