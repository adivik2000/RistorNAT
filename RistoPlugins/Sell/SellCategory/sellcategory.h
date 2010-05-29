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
#ifndef SELLCATEGORY_H
#define SELLCATEGORY_H

#include <plugininterface.h>
#include "ui_sellcategorydetail.h"
class simpleTable;

/** @brief Manage selling category [Sell set]
  *
  * With this plugin you can easily manage the selling categories for
  * the selling articles.
  * @image html sell_category.tiff
  * In the <b>code</b> column, insert a brief code for the category;
  * In the <b>description</b> column, insert the name of the category.
  */
class sellCategory : public pluginInterface {
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    /** @brief Constructor
      * @param parent Parent of the plugin
      */
    sellCategory(QWidget *parent = 0);

    /** @brief Name of the plugin
      * @return Translated plugin's name
      */
    QString name() { return tr("Sell Category"); }

    QIcon icon() { return QIcon(":/categoria_cliente48x48.png"); }

    QString family() { return "Sell"; };

public slots:
    void updateReport();

private:
    simpleTable *m_table;
    Ui::sellCategoryDetail *m_detail;
};

#endif // SELLCATEGORY_H
