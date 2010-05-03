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
#ifndef DINNER_H
#define DINNER_H

#include "workui.h"
#include <QDate>
#include <QtPlugin>

/** @brief Dinner sell [Work set]
  *
  * @par Dependencies
  *
  * This plugin needs:
  * - userInterface, for some operations shared with Lunch
  *
  * @par Description
  *
  * This plugin help you to track all your dinner sell.
  * In the class WorkUi you'll find the manual for the user interface.
  *
  * @see WorkUi
  * @see userInterface
  */
class Dinner : public workUi
{
    Q_OBJECT
public:
    /** @brief Constructor
      * @param parent Parent of the plugin
      */
    Dinner(QWidget *parent = 0): workUi(parent) {
        connect(m_ui->dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(dateChanged(QDate)));
    }

    /** @brief Deconstructor
      *
      */
    ~Dinner() { }

    /** @brief Plugin's name
      * @return Translated plugin's name
      */
    QString name() { return tr("Dinner\nSales"); }

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
  * @param date Date of the dinner
  */
inline void Dinner::editingFinished(QStandardItemModel *model,const QDate& date,
                                    int places)
{
    workUi::saveSell(model,"single_dinner_sell", "dinner_ins_upd",date,places);
}

/** @brief Get the articles sold at certain date
  *
  * @param model In this parameter will return the model of the articles sold,
  * or NULL if there aren't articles
  * @param date Date of the dinner
  */
inline void Dinner::getSoldAtDate(QAbstractItemModel *&model, const QDate& date,
                                  int &places)
{
    workUi::getSell(model,"sold_d_articles","places_for_dinner",date,places);
}

Q_EXPORT_PLUGIN2(dinner, Dinner);
#endif // DINNER_H
