/*
 *  Copyright (c) 2009 Natale Patriciello <kronat@tiscali.it>
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
#ifndef LUNCH_H
#define LUNCH_H

#include "workui.h"
#include <QDate>
#include <QtPlugin>

/** @brief Lunch sell [Work set]
  *
  * @par Dependencies
  *
  * This plugin needs:
  * - userInterface, for some operations shared with Dinner
  *
  * @par Description
  *
  * This plugin help you to track all your lunch sell.
  * In the class WorkUi you'll find the manual for the user interface.
  *
  * @see WorkUi
  * @see userInterface
  */
class Lunch : public workUi
{
    Q_OBJECT
public:
    /** @brief Constructor
      * @param parent Parent of the Plugin
      */
    Lunch(QWidget *parent = 0): workUi(parent) {
        connect(m_ui->dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(dateChanged(QDate)));
    }

    /** @brief Deconstructor
      *
      */
    ~Lunch() { }

    /** @brief Plugin's name
      * @return Translated plugin's name
      */
    QString name() { return tr("Lunch\nSales"); }

    QIcon icon() { return QIcon(":/listino48x48.png"); }

protected slots:
    virtual void editingFinished(QStandardItemModel *model, const QDate& date,
                                 int places);
    virtual void getSoldAtDate(QAbstractItemModel*& model, const QDate& date,
                               int& places);
};

/** @brief Slot called when the user finish the edit
  *
  * Save the articles inserted/modified by the user.
  * @see userInterface::saveSell
  * @param model Model for the articles sold
  * @param date Date of the lunch
  */
inline void Lunch::editingFinished(QStandardItemModel *model, const QDate& date,
                                   int places)
{
    workUi::saveSell(model,"single_lunch_sell",
                            "lunch_ins_upd",date,places);
}

/** @brief Get the articles sold at certain date
  *
  * @param model In this parameter will return the model of the articles sold,
  * or NULL if there aren't articles
  * @param date Date of the lunch
  */
inline void Lunch::getSoldAtDate(QAbstractItemModel *&model, const QDate& date,
                                 int &places)
{
    workUi::getSell(model,"sold_l_articles","places_for_lunch",date,places);
}

Q_EXPORT_PLUGIN2(lunch, Lunch);
#endif // LUNCH_H
