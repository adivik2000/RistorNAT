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
#ifndef UPDATEAMOUNTDELEGATE_H
#define UPDATEAMOUNTDELEGATE_H

#include <workdelegate.h>
#include <QLCDNumber>

class updateAmountPurchDelegate : public doubleSpinBoxDelegate
{
public:
    updateAmountPurchDelegate(QLCDNumber *lcd,QObject *parent = 0):
            doubleSpinBoxDelegate(parent) { m_lcd = lcd; }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

private:
    double m_start;
    QLCDNumber *m_lcd;
};

#endif // UPDATEAMOUNTDELEGATE_H
