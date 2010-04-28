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
#ifndef WORKUI_H
#define WORKUI_H

#include "plugininterface.h"
#include <QtGui/QWidget>
#include <QDate>
#include <QStandardItemModel>
#include <workdelegate.h>
#include <simplecombo.h>

/** @brief Namespace for User Interface */
namespace Ui {
    class workUi;
}

/** @brief Display more detail on Lunch/Dinner [Work set]
  *
  * This interface is called when you click on More Detail in the right-click menu of
  * the class Dinner or Lunch.
  * @image html workUi.tiff
  * Here you could select the date (but it has default on the date that you
  * selected before) and, if no sell exists, insert the total places.\n
  * After you leave the text edit, you could insert the good which you sale;
  * select from the combobox, then press insert, and finally select the
  * quantity (you could increase it by steps of 1) and the selling price,
  * which is automatically retrieved as the last selling cost.\n
  * After you've done your modifications, press Save for saving or Discard
  * to blank the model without saving.
  */
class workUi : public pluginInterface {
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    workUi(QWidget *parent = 0);
    ~workUi();

    QString family() { return "Work"; }

public slots:
    void save();
    void noSave();
    void deleteRow();
    void newRowPressed();
    void addComanda();
    void updateAmount(QStandardItem *item);
    void dateChanged(const QDate& date);

private slots:
    virtual void editingFinished(QStandardItemModel*,const QDate&, int) = 0;
    virtual void getSoldAtDate(QAbstractItemModel*&, const QDate&, int& ) = 0;

protected:
    void changeEvent(QEvent *e);
    QStandardItemModel* makeModel(const QAbstractItemModel* itemModel);
    QStandardItemModel* makeModel();
    void makeMap(QStandardItemModel* model);
    void saveSell(QStandardItemModel *model,const QString& fnInsertSell,
                  const QString& fnInsUpd,const QDate& date, int places);
    void getSell(QAbstractItemModel *&model,const QString& fnGetInformation,
                 const QString& fnGetPlaces, const QDate& date, int &places);

private:
    bool askToSave();
    QMap<QString,int> m_articlesInserted;
    QDate m_date; /**< @brief Date */
    bool m_modified; /**< @brief Indicates if the user has changed
                        * something in the model */
    Ui::workUi *m_ui; /**< User interface */
};


class comboArticleDelegate: public comboBoxDelegate
{
public:
    comboArticleDelegate(QObject *parent):comboBoxDelegate("good_for_sale",0,3,parent) { }

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        comboBoxDelegate::setModelData(editor,model,index);
        simpleCombo *combo = static_cast<simpleCombo*>(editor);
        model->setData(index.sibling(index.row(),0),combo->getPrimaryKeyOfSelected());
        model->setData(index.sibling(index.row(),3),combo->getValueAtColumn(6));
    }
};

#endif // WORKUI_H
