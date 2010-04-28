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
#include <workdelegate.h>


/** @brief Constructor
  *
  * Build a Cost interface.
  */
managementCost::managementCost(QWidget *parent) : pluginInterface(parent)
{
    m_amount = 0.0;
    m_columnPrice = 4;

    ui.setupUi(this);
    ui.line_number->setValidator(new QIntValidator(0,1000000,ui.line_number));
    ui.comboSeller->initialize("supplier",0,0);

    m_documentId = QVariant();

    m_actOk = new QAction(QIcon(":/conferma24x24.png"),tr("Ok"),this);
    m_actUndo = new QAction(QIcon(":/pulisci24x24.png"),tr("Clean"),this);
    m_actDelete = new QAction(QIcon(":/elimina24x24.png"),tr("Delete"),this);
    m_actList = new QAction(QIcon(":/account.png"),tr("List document"),this);

    m_listAction << m_actOk << m_actUndo << m_actDelete << m_actList;

    connect(m_actOk,SIGNAL(triggered()),this,SLOT(okPressed()));
    connect(m_actUndo,SIGNAL(triggered()),this,SLOT(undoPressed()));
    connect(m_actDelete,SIGNAL(triggered()),this,SLOT(deletePressed()));
    connect(m_actList,SIGNAL(triggered()),this,SLOT(listPressed()));

    ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui.tableView,SIGNAL(afterInsert(const QList<QStandardItem*>&)),
            this,SLOT(updateAmountForIns(const QList<QStandardItem*>&)));
    connect(ui.tableView,SIGNAL(afterDelete(const QList<QStandardItem*>&)),
            this,SLOT(updateAmountForDel(const QList<QStandardItem*>&)));
    connect(ui.tableView,SIGNAL(afterEdit(const QList<QStandardItem*>&,
                                          const QList<QStandardItem*>&)),
            this,SLOT(updateAmountForEdit(const QList<QStandardItem*>&,
                                          const QList<QStandardItem*>&)));

    m_comboCategory = new comboBoxDelegate("cost_category",0,0,this);
    m_comboUM = new comboBoxDelegate("unit_of_measurement",0,0,this);
    m_comboGood = new comboBoxDelegate("basic_good",0,0,this);
}

/** @brief Deconstructor
  */
managementCost::~managementCost()
{
    foreach(QAction *act, m_listAction) {
        delete act;
    }
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
  * The function is called when the user click outside, or press tab, in the
  * number line edit. When this occour, this function will search in the archives
  * if exists a document with the param specified in the header section, and
  * populate the table.
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

    if (! m_documentId.isValid())
        return;

    if (radioPurch) {
        ui.tableView->setItemDelegateForColumn(2,m_comboCategory);
    } else {
        ui.tableView->setItemDelegateForColumn(2,m_comboGood);
        ui.tableView->setItemDelegateForColumn(4,m_comboUM);
    }

    ui.tableView->setDefaultValue(1,m_documentId,true);
    ui.tableView->setDefaultValue(0,QVariant(),true);
}

/** @brief Get the information from database
  *
  * Helper function to numberChanged, get the information from the
  * database and set it to the table.
  * @return ID of the document created or retrieved if exist
  * @param functionName name of the database function for retrieving the data
  * about the document
  * @param rowTableName Name of the table which contain the row of the document
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

    m_amount = model->data(model->index(0,0)).toDouble();
    ui.lcdNumber->display(m_amount);

    enableHeader(false);
    delete model;

    return m_documentId;
}

/** @brief Enable the header section
  * @param Enable or disable the header section
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

/** @brief Update amount after an insert
  *
  * @param list New row inserted, which in position 3 has price
  */
void managementCost::updateAmountForIns(const QList<QStandardItem*> &list)
{
    if (list.count()==0)
        return;
    Q_ASSERT(list.count()>m_columnPrice);
    m_amount += list.at(m_columnPrice)->text().toDouble();
    ui.lcdNumber->display(m_amount);
}

/** @brief Update amount after an edit
  *
  * @param before Row before edit
  * @param after Row after edit
  */
void managementCost::updateAmountForEdit(const QList<QStandardItem *> &before,
                                 const QList<QStandardItem *> &after)
{
    if (before.count() == 0 || after.count() == 0)
        return;
    Q_ASSERT(before.count()>m_columnPrice && after.count()>m_columnPrice);
    m_amount -= before.at(m_columnPrice)->text().toDouble();
    m_amount += after.at(m_columnPrice)->text().toDouble();

    ui.lcdNumber->display(m_amount);
}

/** @brief Update amount after a delete
  *
  * @brief list Row deleted
  */
void managementCost::updateAmountForDel(const QList<QStandardItem*> &list)
{
    Q_ASSERT(list.count()>m_columnPrice);
    m_amount -= list.at(m_columnPrice)->text().toDouble();
    ui.lcdNumber->display(m_amount);
}

/** @brief Save the changes
  *
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

    m_documentId = QVariant();
    m_amount = 0.0;
}

/** @brief Delete a document
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
    if (unlikely(!m_documentId.isValid()))
        return;

    paramList param;
    param.append(m_documentId);
    simpleQuery query;
    static char const * const funcName[2] = { "delete_document_goods",
                                              "delete_document_purchasing"};
    query.setFunctionName(funcName[ui.radioPurchasing->isChecked()]);
    query.setParameters(param);
    query.execute();

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
}

Q_EXPORT_PLUGIN2(managementcost, managementCost);
