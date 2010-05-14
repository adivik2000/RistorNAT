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
#include "detail.h"
#include "ui_detail.h"
#include "simplequery.h"

/** @brief Constructor
  * @param parent Parent
  */
goodsDetail::goodsDetail(QWidget *parent) : QGroupBox(parent), ui(new Ui::Detail)
{
    ui->setupUi(this);
}

/** @brief Deconstructor
  */
goodsDetail::~goodsDetail()
{
    delete ui;
}

/** @brief Translate the ui
  */
void goodsDetail::changeEvent(QEvent *e)
{
    QGroupBox::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/** @brief Update the information
  *
  * Perform a query into database to get updated information.
  *
  * @param index Index selected by the user
  */
void goodsDetail::updateInfo(QModelIndex index)
{
    if(!ui->checkBox->isChecked())
        return;

    QVariant article = index.sibling(index.row(),0).data();
    QVariant res;
    Q_ASSERT(article.isValid());
    paramList param;
    param.append(article);

    simpleQuery query("average_cost",param);

    query.execute(res);
    ui->lineAvg->setText(res.toString());
}
