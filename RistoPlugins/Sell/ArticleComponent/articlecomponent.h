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

/** @brief Namespace for UI */
namespace Ui {
    class articleComponentUi;
}

/** @brief Manage composed articles
  *
  * Sometimes, an article is composed by many articles. Here you could
  * specify each basic good which are in a selling article.
  * @image html article_component.png
  *
  */
class articleComponent : public pluginInterface {
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    articleComponent(QWidget *parent = 0);
    ~articleComponent();

    /** @brief Plugin's icon */
    QIcon icon() { return QIcon(":/composizione_listino24x24.png"); }

    /** @brief Name of the plugin
      * @return Translated plugin's name
      */
    QString name() { return tr("Article\nComponent"); }

    /** @brief Plugin's family
      * @return Sell
      */
    QString family() { return "Sell"; }

    void aboutToBeOpened();

public slots:
    void sellingChanged();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::articleComponentUi m_ui; /**< @brief Ui object */
};

#endif // COMPONENTUI_H
