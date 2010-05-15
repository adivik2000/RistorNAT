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
 *   GNU General Public License for more goodsDetails.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RistorNAT.If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GOODSDETAIL_H
#define GOODSDETAIL_H

#include <QGroupBox>
#include <QModelIndex>
#include "ui_detail.h"

namespace Ui {
    class goodsDetail;
}

/** @brief Show a group box with two line edit, showing goodsDetailed information
  *
  * goodsDetailed informations are last cost for selected article, and its average
  * cost.
  */
class goodsDetail : public QGroupBox {
    Q_OBJECT
public:
    goodsDetail(QWidget *parent = 0);
    ~goodsDetail();
public slots:
    void updateInfo(QModelIndex index);
    void checkEnable(int status);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Detail *ui; /**< @brief Ui */
};

/** @brief Enable or disable the widget.
  * @param status Status of the checkBox
  */
inline void goodsDetail::checkEnable(int status)
{
    ui->lineAvg->setEnabled(status);
    ui->labelAvg->setEnabled(status);
}
#endif // GOODSDETAIL_H
