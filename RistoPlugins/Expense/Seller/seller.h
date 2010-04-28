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
#ifndef seller_H
#define seller_H

#include "pluginInterface.h"
#include <QStandardItemModel>
#include "ui_seller.h"

namespace Ui {
    class sellerUi;
}

class seller : public pluginInterface {
    Q_OBJECT
    Q_INTERFACES(pluginInterface);
public:
    seller(QWidget *parent = 0);
    ~seller();

    /** @brief Name of the plugin
     *  @return Plugin name
     */
    virtual QString name() { return tr("Sellers") ; }

    /** @brief Plugin's icon */
    QIcon icon() { return QIcon(":/fornitore48x48.png");  }

    QString family() { return "Expense"; }

public slots:
    void okPressed();
    void askToSave();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::sellerUi ui;
    QStandardItemModel m_viewModel;
};

#endif // seller_H
