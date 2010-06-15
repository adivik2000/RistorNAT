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
#include "managementcost.h"

#include <QMessageBox>
#include <QDialogButtonBox>
#include <QIntValidator>
#include <QtPlugin>

#include <advancedtable.h>
#include <simplequery.h>

/** @brief Constructor
  *
  * Build the interface and create/connect action in the toolbar.
  */
managementCost::managementCost(QWidget *parent) : pluginInterface(parent)
{
    ui.setupUi(this);
    ui.line_number->setValidator(new QIntValidator(0,1000000,ui.line_number));
    ui.comboSeller->initialize("supplier",0,0);

    m_documentId = QVariant();

    QAction *actOk = new QAction(QIcon(":/conferma24x24.png"),tr("Ok"),this);
    QAction *actUndo = new QAction(QIcon(":/pulisci24x24.png"),tr("Clean"),this);
    QAction *actDelete = new QAction(QIcon(":/elimina24x24.png"),tr("Delete"),this);
    QAction *actList = new QAction(QIcon(":/account.png"),tr("List document"),this);

    QToolBar *toolBar = new QToolBar(this);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addAction(actOk);
    toolBar->addAction(actUndo);
    toolBar->addAction(actDelete);
    toolBar->addAction(actList);

    ui.toolLayout->addWidget(toolBar);

    connect(actOk,SIGNAL(triggered()),this,SLOT(okPressed()));
    connect(actUndo,SIGNAL(triggered()),this,SLOT(undoPressed()));
    connect(actDelete,SIGNAL(triggered()),this,SLOT(deletePressed()));
    connect(actList,SIGNAL(triggered()),this,SLOT(listPressed()));
    connect(ui.tableView,SIGNAL(afterSave()),this,SLOT(saveAmountDisplayed()));
    connect(ui.tableView,SIGNAL(beforeDelete()),this,SLOT(delFromAmountDisplayed()));
    connect(ui.tableView,SIGNAL(afterRevert()),this,SLOT(addToAmountDisplayed()));

    ui.tableView->setSelectionMode(QAbstractItemView::ContiguousSelection);

    m_comboCategory = new comboBoxDelegate("cost_category",0,0,this);
    m_comboUM = new comboBoxDelegate("unit_of_measurement",0,0,this);
    m_comboGood = new comboBoxDelegate("basic_good",0,0,this);
    m_purchPriceDel = new updateAmountPurchDelegate(ui.lcdNumber,this);
    m_goodsDel = new totalPriceDelegate(ui.lcdNumber,3,5,this);
}

/** @brief Deconstructor
  */
managementCost::~managementCost()
{

}

/** @brief Retranslate the Ui
  */
void managementCost::changeEvent(QEvent *e)
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

/** @brief Fill the table with the information about the document (if exist)
  *
  * Search in the archives a document with the param specified in the
  * header section, and populate the table if it exists with its information.
  *
  * @see populateTable
  */
void managementCost::goPressed()
{
    bool radioGoods = ui.radioGoods->isChecked();
    bool radioPurch = ui.radioPurchasing->isChecked();

    if ((!radioGoods && !radioPurch) || ui.comboSeller->currentIndex() < 0 ||
        ui.line_number->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Parameter(s) missing"));
        msgBox.setInformativeText(tr("Please select all the parameters."));
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
        return;
    }

    static char const * const funcName[2] = { "document_goods_ins_upd",
                                           "document_purchasing_ins_upd" };
    static char const * const tableName[2] = { "row_goods_cost",
                                            "row_purchasing_cost" };
    static char const * const amountName[2]={ "get_amount_document_goods",
                                              "get_amount_document_purchasing"};

    m_documentId = populateTable(funcName[radioPurch],tableName[radioPurch],
                                 amountName[radioPurch]);

    if (radioPurch) {
        ui.tableView->setItemDelegateForColumn(4,m_purchPriceDel);
        ui.tableView->setItemDelegateForColumn(3,m_comboCategory);
    } else {
        ui.tableView->setItemDelegateForColumn(2,m_comboGood);
        ui.tableView->setItemDelegateForColumn(4,m_comboUM);
        ui.tableView->setItemDelegateForColumn(3,m_goodsDel);
        ui.tableView->setItemDelegateForColumn(5,m_goodsDel);
        for (int i=0;i<ui.tableView->model()->columnCount();i++)
            ui.tableView->setColumnWidth(i,100);
    }

    if ( ! m_documentId.isValid()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Something goes wrong"));
        msgBox.setInformativeText(tr("The document id isn't valid. Your changes"
                                     "will not be saved correctly"));
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
    }

    ui.tableView->setDefaultValue(1,m_documentId,true);
    ui.tableView->setDefaultValue(0,QVariant(),true);

    ui.tableView->setEnabled(true);
}

/** @brief Get the information from database
  *
  * Get the information about the document from the database.
  *
  * @return ID of the document created or retrieved if exist
  * @param functionName name of the database function for retrieving the data
  * about the document
  * @param rowTableName Name of the table which contain the row of the document
  * @param amountFnName Name of the function which saves amount
  */
QVariant managementCost::populateTable(const QString& functionName,
                               const QString& rowTableName,
                               const QString& amountFnName)
{
    QString number = ui.line_number->text();
    if (unlikely(number.isEmpty()))
        return QVariant();

    QAbstractItemModel *model;

    // Insert the header
    paramList param, param2;

    param.append(ui.dateEdit->date());
    param.append(number);
    param.append(ui.comboSeller->getPrimaryKeyOfSelected());

    simpleQuery query(functionName,param);

    if (unlikely((model = query.getResult()) == 0)) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Error executing a query"));
        msgBox.setInformativeText(query.getErrorMessage());
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
        return QVariant();
    }

    m_documentId = model->data(model->index(0,0));

    ui.tableView->setTableName(rowTableName,"document="+m_documentId.toString());

    param2.append(m_documentId);
    simpleQuery query2(amountFnName,param2);

    if (unlikely((model = query2.getResult()) == 0)) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Error executing a query"));
        msgBox.setInformativeText(query.getErrorMessage());
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
    }

    double amount = model->data(model->index(0,0)).toDouble();
    ui.lcdNumber->display(amount);

    enableHeader(false);
    delete model;

    return m_documentId;
}

/** @brief Enable or disable the header section
  * @param enable True if you want to enable, false to disable.
  */
void managementCost::enableHeader(bool enable)
{
    ui.dateEdit->setEnabled(enable);
    ui.line_number->setEnabled(enable);
    ui.comboSeller->setEnabled(enable);
    ui.radioGoods->setEnabled(enable);
    ui.radioPurchasing->setEnabled(enable);
    ui.btnGo->setEnabled(enable);
}

/** @brief Save the changes
  *
  * After saving, clear all things.
  * @see undoPressed
  */
void managementCost::okPressed()
{
    if (likely(ui.tableView->isUsable()))
        ui.tableView->saveChanges();

    undoPressed();
}

/** @brief Clear the ui and the table
  *
  */
void managementCost::undoPressed()
{
    ui.dateEdit->clear();
    ui.line_number->setText("");
    ui.comboSeller->setCurrentIndex(-1);
    ui.lcdNumber->display(0.0);

    enableHeader(true);

    ui.tableView->clear();
    ui.tableView->setEnabled(false);

    m_documentId = QVariant();
}

/** @brief Delete a document from database
  *
  */
void managementCost::deletePressed()
{
    if (!ui.radioGoods->isChecked() && !ui.radioPurchasing->isChecked()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Select a mode"));
        msgBox.setInformativeText(tr(
                "Do you want to delete a purchasing or goods cost?"));
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
        return;
    }

    if (unlikely(!m_documentId.isValid())) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("No document deleted"));
        msgBox.setInformativeText(tr(
                "What document ?!?"));
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
        return;
    }

    paramList param;
    param.append(m_documentId);
    simpleQuery query;
    static char const * const funcName[2] = { "delete_document_goods",
                                              "delete_document_purchasing"};
    query.setFunctionName(funcName[ui.radioPurchasing->isChecked()]);
    query.setParameters(param);
    bool ok = query.execute();
    Q_ASSERT(ok == true);

    undoPressed();
}

/** @brief List all the documents in a new widget
  *
  */
void managementCost::listPressed()
{
    if (!ui.radioGoods->isChecked() && !ui.radioPurchasing->isChecked()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Select a mode"));
        msgBox.setInformativeText(tr(
                "Do you want to delete a purchasing or goods cost?"));
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
        return;
    }

    QDialog dialogBox;
    QGridLayout dialogLayout;
    simpleTable table(this);

    static char const * const tableName[2] = {"document_goods_cost",
                                              "document_purchasing_cost" };

    dialogBox.setWindowTitle(tr("List of documents"));
    dialogBox.setGeometry(100,100,750,300);

    table.setTableName(tableName[ui.radioPurchasing->isChecked()],QString());
    table.setEditTriggers(QAbstractItemView::NoEditTriggers);
    table.setSelectionBehavior(QAbstractItemView::SelectRows);
    table.setSelectionMode(QAbstractItemView::SingleSelection);
    table.resizeColumnsToContents();

    dialogLayout.addWidget(&table);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok
                         | QDialogButtonBox::Cancel,Qt::Horizontal,&dialogBox);

    connect(&buttonBox, SIGNAL(accepted()), &dialogBox, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialogBox, SLOT(reject()));
    connect(&table,SIGNAL(doubleClicked(QModelIndex)),&dialogBox,SLOT(accept()));

    dialogLayout.addWidget(&buttonBox);
    dialogBox.setLayout(&dialogLayout);

    if (likely(dialogBox.exec())) {
        QItemSelectionModel *_selectionModel = table.selectionModel();
        QModelIndexList indexes = _selectionModel->selectedRows();

        if (indexes.count() <= 0)
            return;

        QModelIndex index = indexes.at(0);

        m_documentId = index.sibling(index.row(),0).data();
        QVariant number = index.sibling(index.row(),2).data();
        QVariant date = index.sibling(index.row(),1).data();
        QVariant seller = index.sibling(index.row(),3).data();
        QVariant amount = index.sibling(index.row(),5).data();

        ui.comboSeller->setItemAtPk(seller);
        ui.dateEdit->setDate(date.toDate());
        ui.line_number->setText(number.toString());
        ui.lcdNumber->display(amount.toDouble());

        goPressed();
    }

    for (int i=0;i<ui.tableView->model()->columnCount();i++)
        ui.tableView->setColumnWidth(i,100);
}

/** @brief Save in the database the amount of the document
  *
  */
void managementCost::saveAmountDisplayed()
{
    Q_ASSERT(m_documentId.isValid());
    static char const * const fnName[2] = {"set_amount_document_goods",
                                           "set_amount_document_purchasing" };
    paramList param;
    param.append(m_documentId);
    param.append(ui.lcdNumber->value());
    simpleQuery query(fnName[ui.radioPurchasing->isChecked()],param);

    if ( ! query.execute() ) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Error executing a query"));
        msgBox.setInformativeText(query.getErrorMessage());
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
    }
}

/** @brief Get the amount from the row pointed by the idx
  *
  * @param idx Index for the row.
  * @return the amount
  */
double managementCost::getAmountForRow(QModelIndex idx)
{
    bool ok;
    int columnPrice, columnQty;
    double val = -1;
    if (ui.radioGoods->isChecked()) {
        columnPrice = 5;
        columnQty = 3;
        QVariant price =  idx.sibling(idx.row(),columnPrice).data();
        QVariant qty = idx.sibling(idx.row(),columnQty).data();
        val = price.toDouble(&ok) * qty.toDouble(&ok);

        if (unlikely(!ok))
            val = -1;
    } else if (ui.radioPurchasing->isChecked()) {
        columnPrice = 4;
        QVariant price =  idx.sibling(idx.row(),columnPrice).data();
        val = price.toDouble(&ok);

        if (unlikely(!ok))
            val = -1;
    }

    return val;
}

/** @brief Substract amount of row selected from the amount displayed.
  *
  */
void managementCost::delFromAmountDisplayed()
{
    QItemSelectionModel *_selectionModel = ui.tableView->selectionModel();
    QModelIndexList indexes = _selectionModel->selectedIndexes();
    QVector<int> map;

    foreach (QModelIndex idx, indexes) {
        if (map.contains(idx.row()))
            continue;
        map.append(idx.row());

        double val = getAmountForRow(idx);

        if (val < 0)
            continue;

        ui.lcdNumber->display(ui.lcdNumber->value() - val);
    }

    saveAmountDisplayed();
}

/** @brief Add amount of row selected to the amount displayed.
  *
  */
void managementCost::addToAmountDisplayed()
{
    QItemSelectionModel *_selectionModel = ui.tableView->selectionModel();
    QModelIndexList indexes = _selectionModel->selectedIndexes();

    foreach (QModelIndex idx, indexes) {
        double val = getAmountForRow(idx);

        if (val < 0)
            continue;

        ui.lcdNumber->display(ui.lcdNumber->value() + val);
    }

    saveAmountDisplayed();
}

/** @brief Refresh the combobox when the widget is displayed
  *
  */
void managementCost::aboutToBeOpened()
{
    ui.comboSeller->refresh();
}

Q_EXPORT_PLUGIN2(managementcost, managementCost);
