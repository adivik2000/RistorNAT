/*
 *  Copyright (c) 2009 Natale Patriciello <kronat@tiscali.it>
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
#ifndef GOODSCATEGORY_H
#define GOODSCATEGORY_H

#include <plugininterface.h>
#include <QtPlugin>

/** @brief Manage the good's categories [Sell set]
  *
  * With this plugin you can easily manage the categories of goods.
  * @image html goods_category.tiff
  * - In the <b>code</b> column insert a brief code for the category.
  * - In the <b>description</b> column insert the name of the category.
  *
  * These entries will be referenced by an high number of plugins, like
  * basicGood.
  */
class goodsCategory : public pluginInterface {
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    /** @brief Constructors
      *
      */
    goodsCategory(QWidget *parent = 0);

    /** @brief Plugin name
      * @return Translated plugin name
      */
    QString name() { return tr("Goods Category"); }

    QIcon icon() { return QIcon(":/categoria_articolo48x48.png"); }

    QString family() { return tr("Sell"); }
};

#endif // GOODSCATEGORY_H
