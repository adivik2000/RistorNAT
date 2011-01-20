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
articleComponent::articleComponent(QWidget *parent) :
    pluginInterface(parent),ui(new Ui::articleComponentUi)
{
    ui->setupUi(this);
    ui->comboSelling->initialize("good_for_sale",0,1);

    ui->tableView->setTableName("article_component");
    ui->tableView->addRelation(0,"basic_good","name","name");
    ui->tableView->addRelation(1,"good_for_sale","id","description");
    ui->tableView->addRelation(3,"unit_of_measurement","name","name");

    QToolBar *toolBar = ui->tableView->getToolBar();
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->toolLayout->addWidget(toolBar);
}

/** @brief Deconstructor
  */
articleComponent::~articleComponent()
{
    delete ui;
}

/** @brief Retranslate UI */
void articleComponent::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/** @brief Change articles displayed on the table
  */
void articleComponent::sellingChanged()
{
    if (likely(ui->tableView->isDirty())) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("The table has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                                  QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        int ret = msgBox.exec();

        if (ret == QMessageBox::Save)
                ui->tableView->saveChanges();
    }

    if (unlikely(ui->comboSelling->currentIndex() <= 0)) {
        ui->tableView->setFilter("");
        ui->tableView->resetDefaultValue(1);
        return;
    }

    QVariant pk = ui->comboSelling->getPrimaryKeyOfSelected();
    ui->tableView->setFilter("selling_component="+pk.toString());
    ui->tableView->setDefaultValue(1,pk,true);
}

/** @brief Refresh the combobox each time plugin is displayed */
void articleComponent::aboutToBeOpened()
{
    ui->comboSelling->refresh();
}

Q_EXPORT_PLUGIN2(articlecomponent, articleComponent);
