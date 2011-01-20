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
#include <QDebug>
/** @brief Constructor
  *
  * Set the table to cost_category, and populate the detailed model.
  */
managementCategory::managementCategory(QWidget *parent) : pluginInterface(parent)
{
    simpleQuery query("get_description_from_purch");
    QAbstractItemModel *model_desc = query.getResult();
    Q_ASSERT(model_desc != 0);
    model_desc->setHeaderData(0,Qt::Horizontal,tr("Purchased articles"));

    ui.setupUi(this);

    ui.tableCategory->setTableName("cost_category");
    ui.tableDescription->setModel(model_desc);
    model_desc->setParent(ui.tableDescription);

    m_viewModel.insertColumn(0);
    m_viewModel.insertColumn(1);
    m_viewModel.setHeaderData(0,Qt::Horizontal,tr("Category"));
    m_viewModel.setHeaderData(1,Qt::Horizontal,tr("Amount"));

    ui.tableView->setModel(&m_viewModel);

    QStandardItemModel *model = new QStandardItemModel(this);
    ui.tableViewDetailed->setModel(model);

    connect(ui.tableView,SIGNAL(pressed(QModelIndex)),this,
            SLOT(showCategoryDetail(QModelIndex)));
    connect(ui.tableCategory,SIGNAL(pressed(QModelIndex)),this,
            SLOT(categoryClicked()));
    connect(ui.tableDescription,SIGNAL(pressed(QModelIndex)),this,
            SLOT(descriptionClicked()));

    QToolBar *toolBar = ui.tableCategory->getToolBar();
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui.toolLayout->addWidget(toolBar);
}

/** @brief Deconstructor
  */
managementCategory::~managementCategory()
{
}

/** @brief Retranslate UI
  * @param e event
  */
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

void managementCategory::categoryClicked()
{
    m_table_active[0] = true;
    m_table_active[1] = false;
    ui.tableDescription->selectionModel()->clearSelection();
}

void managementCategory::descriptionClicked()
{
    m_table_active[0] = false;
    m_table_active[1] = true;
    ui.tableCategory->selectionModel()->clearSelection();
}

void managementCategory::okPressed()
{
    if (m_table_active[0] == true) {
        report(*ui.tableCategory, "report_management_category");
    } else if (m_table_active[1] == true) {
        report(*ui.tableDescription,"report_purchasing_article");
    }
}

/** @brief Display a general report model
  *
  * The report has two columns: Category, which references category, and
  * Amount, which references the total amount for the category, in the period
  * selected by the user.
  */
void managementCategory::report(const QTableView &table, const QString &fn)
{
    int rowDone = 0;

    QItemSelectionModel *_selectionModel = table.selectionModel();
    QModelIndexList indexes = _selectionModel->selectedRows();
    QModelIndex index;
    QStandardItemModel *modelView = qobject_cast<QStandardItemModel*>(
            ui.tableView->model());
    QAbstractItemModel *modelDetailed = ui.tableViewDetailed->model();

    modelView->removeRows(0,modelView->rowCount());
    delete modelDetailed;

    foreach(index, indexes)
    {
        QVariant value = index.data();

        paramList param;
        param.append(QVariant(ui.dateFrom->date()));
        param.append(QVariant(ui.dateTo->date()));
        param.append(value);

        simpleQuery query(fn,param);

        QAbstractItemModel *model = query.getResult();

        if (likely (model != 0 && model->rowCount() == 1)) {
            QVariant amount = model->data(model->index(0,0));
            if (! amount.isValid()) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(tr("No information"));
                msgBox.setInformativeText(tr("There aren't information for ")
                                             + value.toString());
                msgBox.setStandardButtons(QMessageBox::Ok);

                msgBox.exec();
                continue;
            }
            QStandardItem *item = new QStandardItem(value.toString());
            QStandardItem *item2 = new QStandardItem(amount.toString() + " $");

            modelView->setItem(rowDone,0,item);
            modelView->setItem(rowDone,1,item2);

            rowDone++;
            delete model;
        }
    }
}

/** @brief Display detail about a category and its amount
  *
  * If the user clicks on a category in the report model, another
  * detailed report appear, with each documents (and the import) which contribuite
  * to the amount for the period selected.
  */
void managementCategory::showCategoryDetail(QModelIndex index)
{
    if (m_table_active[1] == true) {
        return;
    }
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

/** @brief If the table is modified, ask to save information before quit.
  *
  */
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
