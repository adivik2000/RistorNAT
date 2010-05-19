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
#include <QDebug>

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

/** @brief Set data in the model, update amount on the lcd
  *
  * The delegate is born to work only with a row_goods_cost table.
  * If it isn't placed on column 3 or 5 (qty and price) it doesn't do nothing
  * more that a DoubleSpinBoxDelegate.
  *
  * @param editor Editor (doublespinbox)
  * @param model Model
  * @param index Index modified
  */
void updateAmountGoodsDelegate::setModelData(QWidget *editor,
                                             QAbstractItemModel *model,
                                             const QModelIndex &index) const
{
    bool ok;
    double qty = 0.0, price = 0.0, amount = 0.0;
    QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(editor);
    updateAmountGoodsDelegate *self = const_cast<updateAmountGoodsDelegate*>(this);
    spin->interpretText();

    if (index.column() == 3) { // User edit qty
        QModelIndex idx = index.sibling(index.row(),5);
        price = idx.data().toDouble(&ok);

        if (unlikely(!ok))
            goto exit_clean;

        qty = spin->value();
        amount = self->m_lcd->value() - price*(self->m_startQty - qty);
    } else if (index.column() == 5) { // User edit price
        QModelIndex idx = index.sibling(index.row(),3);
        qty = idx.data().toDouble(&ok);

        if (unlikely(!ok))
            goto exit_clean;

        price = spin->value();
        amount = self->m_lcd->value() - (self->m_startPrice - price)*qty;
    } else
        goto exit_clean;

    self->m_lcd->display(amount);

exit_clean:
    return doubleSpinBoxDelegate::setModelData(editor,model,index);
}

/** @brief Set editor data
  *
  * Initialize also the amount from the LCD display
  *
  * @param editor Editor (doublespinbox)
  * @param index Index
  */
void updateAmountGoodsDelegate::setEditorData(QWidget *editor,
                                              const QModelIndex &index) const
{
    double value = index.data().toDouble();
    updateAmountGoodsDelegate *self = const_cast<updateAmountGoodsDelegate*>(this);
    if (index.column() == 5) { // User edit price
        self->m_startPrice = value;
    } else if (index.column() == 3) { // User edit qty
        self->m_startQty = value;
    }

    return doubleSpinBoxDelegate::setEditorData(editor,index);
}
