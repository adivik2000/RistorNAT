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
#include "stackedwcontainer.h"

#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QMessageBox>

/** @brief Create a root-plugin and loads its sub-plugins
  *
  * @param parent Parent
  * @param iconName Icon name
  * @param path Path of sub-plugins
  * @param iconName Plugin icon (only the name)
  */
stackedWContainer::stackedWContainer(QWidget *parent):QWidget(parent)
{
    m_position = 0;

    QHBoxLayout *pageLayout = new QHBoxLayout(this);
    m_pagesWidget = new QStackedWidget(this);
    m_contentsWidget = new QListWidget(this);

    connect(m_contentsWidget,
           SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
           this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));

    m_contentsWidget->setViewMode(QListView::IconMode);
    m_contentsWidget->setIconSize(QSize(240, 240));
    m_contentsWidget->setMovement(QListView::Static);
    m_contentsWidget->setMaximumWidth(128);
    m_contentsWidget->setSpacing(12);

    pageLayout->addWidget(m_contentsWidget);
    pageLayout->addWidget(m_pagesWidget);

    this->setLayout(pageLayout);
}

/** @brief Deconstructor
  *
  * Delete the root plugin, icons in the toolbar, all the actions and
  * all the childs.
  */
stackedWContainer::~stackedWContainer()
{
    foreach (pluginInterface *child, m_plugins) {
        delete child;
    }
    delete m_contentsWidget;
    delete m_pagesWidget;
}

void stackedWContainer::add(pluginInterface *child)
{
    if (likely(child != 0)) {
        QListWidgetItem *item = new QListWidgetItem(m_contentsWidget);

        QIcon icon = child->icon();

        item->setText(child->name());
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setIcon(icon);

        m_plugins.append(child);
    } else {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Can't load a plugin"));
        msgBox.setInformativeText(child->name());
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
    }
}

/** @brief Change the current displayed sub-plugins
  *
  * Clean the resources obtained by the plugin that need to be closed, then
  * open the new plugin.
  *
  * @see clean
  * @param current Curret sub-plugins displayed
  * @param previous Previous sub-plugin displayed
  */
void stackedWContainer::changePage(QListWidgetItem *current,
                                   QListWidgetItem *previous)
{
    if (!current)
         current = previous;

    pluginInterface *widgetToClose = qobject_cast<pluginInterface*>(
            m_pagesWidget->currentWidget());

    aboutToBeClosed();

    m_pagesWidget->removeWidget(widgetToClose);

    m_position = m_contentsWidget->row(current);

    if (likely(m_position != -1)) {
        pluginInterface *child = m_plugins.at(m_position);
        Q_ASSERT(child != 0);
        child->aboutToBeOpened();
        m_pagesWidget->insertWidget(m_position, child);
        m_pagesWidget->setCurrentIndex(m_position);
    }
}

/** @brief Open the previously clicked item
  *
  */
void stackedWContainer::aboutToBeOpened()
{
    QListWidgetItem *item = m_contentsWidget->item(m_position);
    m_contentsWidget->setCurrentItem(item);

    pluginInterface *child = m_plugins.at(m_position);

    if (child != 0)
        child->aboutToBeOpened();
}

void stackedWContainer::aboutToBeClosed()
{
    pluginInterface *widgetToClose = qobject_cast<pluginInterface*>(
            m_pagesWidget->currentWidget());

    if (widgetToClose != 0)
        widgetToClose->aboutToBeClosed();
}
