/*
 *  Copyright (c) 2009 Natale Patriciello <kronat@tiscali.it>
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
#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QWidget>
#include <QIcon>
#include <QToolBar>
#include <QList>
#include <QAction>

/** @brief
  */
class pluginInterface : public QWidget
{
public:
    pluginInterface(QWidget *parent = 0):QWidget(parent) { }

    /** @brief Default deconstructor */
    virtual ~pluginInterface() { }

    /** @brief Return the name of the plugin
      * @return Plugin name
      */
    virtual QString name() = 0;

    /** @brief Return the icon of the plugin
      *
      * Every plugin needs its own icon. The icon can be NULLed (default
      * constructor of QIcon) but not zeroed.
      * @param path Path for the icons
      * @return Plugin icon
      */
    virtual QIcon icon() = 0;

    /** @brief Register the tool bar, to let the plugin to add or remove
      * custom button.
      * @param toolBar Valid QToolBar
      */
    virtual void registerToolBar(QToolBar *toolBar) {
        Q_ASSERT(toolBar != 0);
        m_toolBar = toolBar;
    }

    virtual QString family() = 0;

    virtual void aboutToBeOpened() { }
    virtual void aboutToBeClosed() { }

protected:
    QToolBar *m_toolBar;
    QList<QAction*> m_listAction;
};

Q_DECLARE_INTERFACE(pluginInterface,
           "com.yourcompany.Plugin.pluginInterface/1.0");
#endif // PLUGININTERFACE_H
