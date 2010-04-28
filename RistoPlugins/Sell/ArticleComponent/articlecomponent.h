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
#ifndef COMPONENTUI_H
#define COMPONENTUI_H

#include <QWidget>
#include <QList>
#include <QAction>
#include <QModelIndex>
#include "plugininterface.h"
#include "ui_articlecomponent.h"

namespace Ui {
    class articleComponentUi;
}

class articleComponent : public pluginInterface {
    Q_OBJECT
public:
    articleComponent(QWidget *parent = 0);
    ~articleComponent();

    QIcon icon() { return QIcon(":/composizione_listino24x24.png"); }

    /** @brief Name of the plugin
      * @return Translated plugin's name
      */
    QString name() { return tr("Article\nComponent"); }

    QString family() { return "Sell"; }

public slots:
    void sellingChanged();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::articleComponentUi *ui;
};

#endif // COMPONENTUI_H
