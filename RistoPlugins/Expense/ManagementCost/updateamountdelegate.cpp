#include "updateamountdelegate.h"
#include <QDoubleSpinBox>
#include <QDebug>

void updateAmountDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(editor);
    spin->interpretText();
    updateAmountDelegate *self = const_cast<updateAmountDelegate*>(this);
    double amount = self->m_lcd->value();
    amount -= self->m_start-spin->value();
    self->m_lcd->display(amount);
    return doubleSpinBoxDelegate::setModelData(editor,model,index);
}

void updateAmountDelegate::setEditorData(QWidget *editor,
                                         const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();
    updateAmountDelegate *self = const_cast<updateAmountDelegate*>(this);
    self->m_start = value;
    return doubleSpinBoxDelegate::setEditorData(editor,index);
}
