#ifndef UPDATEAMOUNTDELEGATE_H
#define UPDATEAMOUNTDELEGATE_H

#include <workdelegate.h>
#include <QLCDNumber>

class updateAmountDelegate : public doubleSpinBoxDelegate
{
public:
    updateAmountDelegate(QLCDNumber *lcd,QObject *parent = 0):
            doubleSpinBoxDelegate(parent) { m_lcd = lcd; }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

private:
    double m_start;
    QLCDNumber *m_lcd;
};

#endif // UPDATEAMOUNTDELEGATE_H
