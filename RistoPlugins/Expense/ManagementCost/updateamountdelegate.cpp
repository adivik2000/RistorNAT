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
#include "updateamountdelegate.h"
#include <QDoubleSpinBox>

/** @brief Set data in the model, update amount on the lcd
  *
  * @param editor Editor (doublespinbox)
  * @param model Model
  * @param index Index modified
  */
void updateAmountPurchDelegate::setModelData(QWidget *editor,
                                             QAbstractItemModel *model,
                                             const QModelIndex &index) const
{
    QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(editor);
    spin->interpretText();

    updateAmountPurchDelegate *self = const_cast<
                                      updateAmountPurchDelegate*>(this);
    double amount = self->m_lcd->value();

    amount -= self->m_start-spin->value();
    self->m_lcd->display(amount);

    return doubleSpinBoxDelegate::setModelData(editor,model,index);
}

/** @brief Set editor data
  *
  * Initialize also the amount from the LCD display
  * @param editor Editor (doublespinbox)
  * @param index Index
  */
void updateAmountPurchDelegate::setEditorData(QWidget *editor,
                                              const QModelIndex &index) const
{
    double value = index.data().toDouble();
    updateAmountPurchDelegate *self = const_cast<
                                      updateAmountPurchDelegate*>(this);
    self->m_start = value;

    return doubleSpinBoxDelegate::setEditorData(editor,index);
}
