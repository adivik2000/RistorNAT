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

#include "simplecombo.h"
#include "simplequery.h"
#include "workdelegate.h"

#include "workui.h"
#include "ui_workui.h"

#include <QModelIndex>
#include <QMessageBox>

/** @brief Constructor
  *
  * @param parent Parent of the widget
  */
workUi::workUi(QWidget *parent) :
    pluginInterface(parent), m_ui(new Ui::workUi)
{
    m_modified = false;
    m_ui->setupUi(this);

    spinBoxDelegate *spinDel2 = new spinBoxDelegate(m_ui->tableEverything);
    m_ui->tableEverything->setItemDelegateForColumn(2,spinDel2);

    totalPriceDelegate *spinDel = new totalPriceDelegate(m_ui->lcdAmount,
                                                         2,3,m_ui->tableComanda);
    comboBoxDelegate *comboDel = new comboArticleDelegate(m_ui->tableComanda);

    m_ui->tableComanda->setItemDelegateForColumn(1,comboDel);
    m_ui->tableComanda->setItemDelegateForColumn(2,spinDel);
    m_ui->tableComanda->setItemDelegateForColumn(3,spinDel);
}

/** @brief Deconstructor
  *
  * Delete the widget
  */
workUi::~workUi()
{
    delete m_ui;
}

/** @brief Re-translate the UI
  *
  */
void workUi::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/** @brief Make a standard model for the articles
  *
  * The standard model has 4 columns, <b>Article Number, Article, Quantity and Single price
  * </b> that could be filled by the commands in the widget.\n
  * If is passed a model with articles, the function fills the standard model returned with these
  * information.
  * @return QStandardItemModel for the articles with the tableView as parent
  */
QStandardItemModel* workUi::makeModel()
{
    QStandardItemModel *model = new QStandardItemModel;

    model->insertColumns(0,4);
    model->setHeaderData(0,Qt::Horizontal,tr("Code"));
    model->setHeaderData(1,Qt::Horizontal,tr("Article"));
    model->setHeaderData(2,Qt::Horizontal,tr("Quantity"));
    model->setHeaderData(3,Qt::Horizontal,tr("Single price"));

    return model;
}

/** @brief
  *
  * Make a model to display from existing articles.
  * @return QStandardItemModel for articles
  *
  * @param itemModel Model with information about articles
  */
QStandardItemModel* workUi::makeModel(const QAbstractItemModel *itemModel)
{
    QStandardItemModel *model = makeModel();

    if (itemModel == 0)
        return model;

    for (int i = 0; i < itemModel->rowCount(); i++) {
        QList<QStandardItem*> list;

        for (int j=0; j < itemModel->columnCount(); j++ ) {
            QModelIndex index = itemModel->index(i,j);
            QStandardItem *item = new QStandardItem(index.data().toString());
            if (j==0 || j==1)
                item->setEditable(false);
            list.append(item);
        }
        model->appendRow(list);
    }

    return model;
}

/** @brief Display a messageBox asking to save the data
  *
  * @see finish
  */
bool workUi::askToSave()
{
    if (m_modified == true) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("The table has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                                  QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        if (msgBox.exec() == QMessageBox::Save)
            return true;
    }

    return false;
}

/** @brief Don't save the model, and delete the data.
  *
  */
void workUi::noSave()
{
    QAbstractItemModel *old_comanda = m_ui->tableComanda->model();
    QAbstractItemModel *old_every = m_ui->tableEverything->model();
    QAbstractItemModel *new_comanda = makeModel();
    QAbstractItemModel *new_every = makeModel();

    new_comanda->setParent(m_ui->tableComanda);
    new_every->setParent(m_ui->tableEverything);

    m_ui->tableComanda->setModel(new_comanda);
    m_ui->tableEverything->setModel(new_every);

    delete old_comanda;
    delete old_every;
}

/** @brief Ask for saving data
  *
  */
void workUi::save()
{
    QStandardItemModel *model;
    model = qobject_cast<QStandardItemModel*>(m_ui->tableEverything->model());
    Q_ASSERT(model != 0);

    editingFinished(model,m_date,m_ui->spinPlaces->value());

    m_modified = false;
}
#include <QDebug>
/** @brief Get a new information for the new date
  *
  * With the signal dateHasChanged, the function will convert the model
  * which is filled by the signal handler in a way which is displayed
  * in the Tableview.
  * @param date The new date
  */
void workUi::dateChanged(const QDate &date)
{
    m_ui->lcdAmount->display(0.0);
    if (askToSave()) {
        save();
    }

    QAbstractItemModel *query_model = 0;
    QAbstractItemModel *old_every = m_ui->tableEverything->model();
    QAbstractItemModel *old_comanda = m_ui->tableComanda->model();
    QStandardItemModel *new_every = 0, *new_comanda=0;
    int places = -1;

    getSoldAtDate(query_model,date,places);

    new_every = makeModel(query_model);
    new_every->setParent(m_ui->tableEverything);
    m_ui->tableEverything->setModel(new_every);
    makeMap(new_every);

    new_comanda = makeModel();
    new_comanda->setParent(m_ui->tableComanda);
    m_ui->tableComanda->setModel(new_comanda);

    m_ui->spinPlaces->setValue(places);

    m_date = date;
    delete old_every;
    delete old_comanda;
}

void workUi::newRowPressed()
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(
            m_ui->tableComanda->model());
    Q_ASSERT(model != 0);

    QStandardItem *item = new QStandardItem();
    QStandardItem *item2 = new QStandardItem();
    QStandardItem *item3 = new QStandardItem();
    QStandardItem *item4 = new QStandardItem();

    QList<QStandardItem*> list;
    list << item << item2 << item3 << item4;

    item->setEditable(false);

    model->appendRow(list);
}

void workUi::makeMap(QStandardItemModel *model)
{
    for (int i=0;i<model->rowCount(); i++) {
        QVariant code = model->data(model->index(i,0));
        m_articlesInserted.insert(code.toString(),i);
    }
}

/** @brief Remove a row from the table everything
  */
void workUi::deleteRow()
{
    QAbstractItemModel *model = m_ui->tableEverything->model();
    QItemSelectionModel *_selectionModel = m_ui->tableEverything->selectionModel();
    QModelIndexList indexes = _selectionModel->selectedRows();
    QModelIndex index;

    foreach (index, indexes) {
        QVariant code = model->data(model->index(index.row(),0));
        m_articlesInserted.take(code.toString());
        m_ui->tableEverything->model()->removeRow(index.row());
    }

    m_modified = true;

    if (indexes.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.warning(0,"",tr("Select one or more rows."),QMessageBox::Ok);
        m_modified = false;
    }
}

/** @brief
  */
void workUi::addComanda()
{
    QStandardItemModel *modelEverything, *modelComanda;
    modelComanda = qobject_cast<QStandardItemModel*>(m_ui->tableComanda->model());
    modelEverything = qobject_cast<QStandardItemModel*>(m_ui->tableEverything->model());
    Q_ASSERT(modelComanda != 0 && modelEverything != 0);

    m_ui->spinPlaces->setValue(m_ui->spinPlaces->value()+m_ui->spinLocalPlaces->value());

    for (int i=0;i<modelComanda->rowCount();i++) {
        QVariant codeToInsert = modelComanda->data(modelComanda->index(i,0));
        QVariant articleToInsert = modelComanda->data(modelComanda->index(i,1));
        QVariant qtyToInsert = modelComanda->data(modelComanda->index(i,2));
        QVariant priceToInsert = modelComanda->data(modelComanda->index(i,3));

        if (codeToInsert.toString().isEmpty())
            continue;

        if (m_articlesInserted.contains(codeToInsert.toString())) {
            int row = m_articlesInserted.value(codeToInsert.toString());
            QVariant qtyToUpdate = modelEverything->data(modelEverything->index(row,2));
            qtyToUpdate = qtyToUpdate.toInt() + qtyToInsert.toInt();
            modelEverything->setData(modelEverything->index(row,2),qtyToUpdate);
        } else {
            QStandardItem *item = new QStandardItem();
            QStandardItem *item2 = new QStandardItem();
            QStandardItem *item3 = new QStandardItem();
            QStandardItem *item4 = new QStandardItem();

            QList<QStandardItem*> list;
            list << item << item2 << item3 << item4;

            item->setText(codeToInsert.toString());
            item2->setText(articleToInsert.toString());
            item3->setText(qtyToInsert.toString());
            item4->setText(priceToInsert.toString());

            item->setEditable(false);
            item2->setEditable(false);

            modelEverything->appendRow(list);
            m_articlesInserted.insert(codeToInsert.toString(),modelEverything->rowCount()-1);
        }
    }

    modelComanda->removeRows(0,modelComanda->rowCount());
    m_ui->spinLocalPlaces->setValue(0);
    m_ui->lcdAmount->display(0);
    m_modified = true;
}

/** @brief Save the sells
  *
  * Save the single sell in model with the function fnInsertSells,
  * checking if the reference for it exists with fnInsUpd.
  * @param model Model with the data
  * @param fnInsertSells Function to insert a single dinner or lunch sell
  * @param fnInsUpd Function to insert or update a dinner or luch
  * @param date Date for the dinner or lunch
  * @param places Places for the date
  */
void workUi::saveSell(QStandardItemModel *model,
                             const QString& fnInsertSells,
                             const QString& fnInsUpd, const QDate& date,
                             int places)
{
    Q_ASSERT(model != 0);
    Q_ASSERT(fnInsertSells == "single_lunch_sell" ||
             fnInsertSells == "single_dinner_sell");
    Q_ASSERT(model->columnCount() == 4);
    Q_ASSERT(date.isValid());

    paramList list;

    list.append(date);
    list.append(places);
    simpleQuery query(fnInsUpd,list);

    if ( unlikely(! query.execute()) ) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.warning(0,"",query.getErrorMessage(),QMessageBox::Ok);
        return;
    }

    query.setFunctionName(fnInsertSells);

    for (int i=0; i<model->rowCount(); i++) {
        paramList list;

        list << date << model->index(i,0).data() << model->index(i,2).data() <<
                model->index(i,3).data();

        query.setParameters(list);

        if (unlikely( ! query.execute() )) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.warning(0,"",query.getErrorMessage(),QMessageBox::Ok);
            return;
        }
    }
}

/** @brief Get a sell for a date
  *
  * @param model Model to fill with the sell information
  * @param fnGetInformation Function to get information about a sell
  * @param fnGetPlaces Function to retrieve places for a date
  * @param date Date for the sell
  * @param places Places for the date
  */
void workUi::getSell(QAbstractItemModel *&model,
                            const QString &fnGetInformation,
                            const QString& fnGetPlaces,const QDate& date,
                            int &places)
{
    Q_ASSERT(date.isValid());

    QAbstractItemModel *places_model;

    paramList list;
    list.append(date);

    simpleQuery query(fnGetPlaces,list);
    places_model = query.getResult();

    if (unlikely(places_model==0)) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.warning(0,"",query.getErrorMessage(),QMessageBox::Ok);
        return;
    }

    places = places_model->data(places_model->index(0,0)).toInt();
    delete places_model;

    query.setFunctionName(fnGetInformation);
    query.setParameters(list);

    model = query.getResult();

    if (unlikely(model==0)) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.warning(0,"",query.getErrorMessage(),QMessageBox::Ok);
    }
}
