/*
 *  Copyright (c) 2010 Natale Patriciello <kronat@tiscali.it>
 *
 *  This file is part of NATManagementSoftware.
 *
 *   NATManagementSw is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   NATManagementSoftware is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NATManagementSoftware.If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef STACKEDWCONTAINER_H
#define STACKEDWCONTAINER_H

#include "plugininterface.h"

#include <QStackedWidget>
#include <QListWidget>
#include <QPointer>

/** @brief Loader plugin
  *
  * This class instead offer support when there are many plugins that load
  * other plugins: the class, assuming that there is a container widget
  * (like QTabWidget), that give one its portion (like a page of QTabWidget)
  * loads other sub-plugins, and then create the icon in the
  * icon toolbar.
  *
  * The creation of a display widget is demanded to the plugin
  * (often they are subclasses of stackedWTable, but can be used others
  * implementation of stackedWEmpty).
  * @see stackedWEmpty
  */
class stackedWContainer : public QWidget
{
    Q_OBJECT
public:
    stackedWContainer(QWidget* parent = 0);

    virtual ~stackedWContainer();

    void aboutToBeOpened();
    void aboutToBeClosed();
    void add(pluginInterface *child);

protected slots:
    virtual void changePage(QListWidgetItem *current, QListWidgetItem *previous);

protected:
    QPointer<QStackedWidget> m_pagesWidget; /**< @brief Container of child
                                             *  plugins widgets */
    QPointer<QListWidget> m_contentsWidget; /**< @brief Icon ToolBox to switch
                                              *  child */
    QList<pluginInterface*> m_plugins; /**< @brief List of child */

private:
    int m_position; /**< @brief Position of the current child in the list */
};

#endif // STACKEDWCONTAINER_H
