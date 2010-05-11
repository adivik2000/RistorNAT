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

void updateAmountPurchDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(editor);
    spin->interpretText();
    updateAmountPurchDelegate *self = const_cast<updateAmountPurchDelegate*>(this);
    double amount = self->m_lcd->value();
    amount -= self->m_start-spin->value();
    self->m_lcd->display(amount);
    return doubleSpinBoxDelegate::setModelData(editor,model,index);
}

void updateAmountPurchDelegate::setEditorData(QWidget *editor,
                                         const QModelIndex &index) const
{
    double value = index.data().toDouble();
    updateAmountPurchDelegate *self = const_cast<updateAmountPurchDelegate*>(this);
    self->m_start = value;
    return doubleSpinBoxDelegate::setEditorData(editor,index);
}

void updateAmountGoodsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(editor);
    spin->interpretText();
    bool ok;
    QModelIndex idx = index.sibling(index.row(),3);
    double qty = idx.data().toDouble(&ok);

    if (unlikely(!ok))
        qty = 1;

    updateAmountGoodsDelegate *self = const_cast<updateAmountGoodsDelegate*>(this);
    double amount = self->m_lcd->value();

    amount -= (self->m_start - spin->value())*qty;
    self->m_lcd->display(amount);

    return doubleSpinBoxDelegate::setModelData(editor,model,index);
}

void updateAmountGoodsDelegate::setEditorData(QWidget *editor,
                                         const QModelIndex &index) const
{
    double value = index.data().toDouble();
    updateAmountGoodsDelegate *self = const_cast<updateAmountGoodsDelegate*>(this);
    self->m_start = value;
    return doubleSpinBoxDelegate::setEditorData(editor,index);
}
