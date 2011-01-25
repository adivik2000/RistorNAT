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

#include "articlecomponent.h"
#include "simplequery.h"
#include <QMessageBox>
#include <QDebug>
#include <QtPlugin>

/** @brief Constructor
  *
  * @param parent Parent
  */
articleComponent::articleComponent(QWidget *parent) : pluginInterface(parent)
{
    m_ui.setupUi(this);
    m_ui.comboSelling->initialize("good_for_sale",0,1);

    m_ui.tableView->setTableName("article_component");
    m_ui.tableView->addRelation(0,"basic_good","name","name");
    m_ui.tableView->addRelation(1,"good_for_sale","id","description");
    m_ui.tableView->addRelation(3,"unit_of_measurement","name","name");

    QStringList header;
    header << tr("Basic Component") << tr("Selling Component")
                << tr("Quantity") << tr("UM");
    m_ui.tableView->setHeader(header);

    QToolBar *toolBar = m_ui.tableView->getToolBar();
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_ui.toolLayout->addWidget(toolBar);
}

/** @brief Deconstructor
  */
articleComponent::~articleComponent()
{
}

/** @brief Retranslate UI */
void articleComponent::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

/** @brief Change articles displayed on the table
  */
void articleComponent::sellingChanged()
{
    if (likely(m_ui.tableView->isDirty())) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("The table has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                                  QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        int ret = msgBox.exec();

        if (ret == QMessageBox::Save)
                m_ui.tableView->saveChanges();
    }

    if (unlikely(m_ui.comboSelling->currentIndex() <= 0)) {
        m_ui.tableView->setFilter("");
        m_ui.tableView->resetDefaultValue(1);
        return;
    }

    QVariant pk = m_ui.comboSelling->getPrimaryKeyOfSelected();
    m_ui.tableView->setFilter("selling_component="+pk.toString());
    m_ui.tableView->setDefaultValue(1,pk,true);
}

/** @brief Refresh the combobox each time plugin is displayed */
void articleComponent::aboutToBeOpened()
{
    m_ui.comboSelling->refresh();
}

Q_EXPORT_PLUGIN2(articlecomponent, articleComponent);
