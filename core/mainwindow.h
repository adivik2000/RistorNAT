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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QTabWidget>
#include <QObject>
#include <QTabWidget>
#include <QList>
#include "connection.h"
#include "settingsui.h"
#include "plugininterface.h"

class stackedWContainer;

/** @author Natale Patriciello, 2009-2010
  * @mainpage RistorNAT
  *
  * @image html article_component.png
  * RistorNAT is a GPL software to manage a restaurant. It can do various things:
  * - It can handle expense's side, allowing you to insert, report and do
  * statistics about all your expenses;
  * - It can handle selling's side, allowing you to manage all your selling
  * articles, and generate report and statistics about article's price;
  * - It can store all about your working days, like places and selling.
  *
  * All things with a nice User Interface, developed with the maximun attention
  * to the User Experience.
  *
  * @section Download
  * Refer to your personal agent to obtain a copy of RistorNAT.
  *
  * @section Installation
  * RistorNAT exist for OSX and for Linux. A Windows XP porting is in progress.
  *
  * For OSX edition, just double-click on the RistorNAT.dmg package, and move
  * RistorNAT in your Applications folder.
  *
  * For Linux edition, install the precompiled package that you've received
  * with your distro's tool. At the moment, there are packages for:
  * - ArchLinux [link]
  * - Debian, Ubuntu [link]
  *
  * @section Dependencies
  *
  * RistorNAT store information in PostgreSQL database. It is built on the
  * Nokia's Qt framework.
  *
  * @section Release History
  *
  * -# 0.4.0 : The Christmas bug (25/12/2009)
  * -# 0.4.1 : The First 0x7DA Release (13/1/2010)
  * -# 0.4.2 : Fast&Fuori Release (6/3/2010)
  * -# 0.5.0 : TrueHell (20/01/2011)
  *
  * @section RoadMap
  *
  * @par 0.5.1
  *
  * -# Complementary sells
  *
  * @section ChangeLog
  *
  * @par 0.5.0
  *
  * -# Added last and average cost for every Goods for sale (GoodsForSale)
  * -# Added statistics for the management category (MngmCostReport)
  * -# Added statistics for sellers (SellersReport)
  * -# Added actual stock and history stock
  *
  * @section Support
  * For RistorNAT is available only a commercial support.
  * Contact your agent to obtain more information.
  *
  * You can see the list of agent [here]
  *
  * @section Plugins
  * RistorNAT is built with modularity in mind: its design keep writing plugin
  * much simple; so it is very simple extend RistorNAT with your request.
  *
  * All features are implemented as plugin, so you can easily enable/disable
  * wanted features. If a feature isn't available, contact your agent.
  * The standard set of plugins contains:
  *
  * - Expense
  * - Sell
  * - Statistics
  * - Work
  *
  * To know more about these, just click and see its documentation.
  *
  */

/** @todo Se il costo di un art e' x, lo vendo a y, voglio la % di guadagno. FOOD COST %*/
/** @todo Aggiungere un modulo di riepilogo, uscite / entrate, decisamente modulare */
/** @todo Molto piu' avanti: COSTO PERSONALE e gestione IVA */

/** @brief User Interface namespace
  * Namespace for all the User Interface things, like widget.\n
  * Use only to separate GUI from core operations.
  * @see MainWindow
  */
namespace Ui
{
    class MainWindow;
}

/** @brief MainWindow class for displaying application's plugin
  *
  * The class MainWindow is the first called, and it only paint the
  * main window form, and load the first application available.\n
  * It is possible to load only one application-plugins.\n
  * The MainWindow class also include some application-indepented feature,
  * like tuning a database, backup it, and open/save/load settings for each
  * application, like the parameters of database or window position.
  */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool setupDatabase();

private slots:
    void openSettings();
    void about();
    void pageChanged(int index);
    void dump();

private:
    Ui::MainWindow *ui; /**< @brief GUI */
    QPointer<settingsUi> settingWindow; /**< @brief Settings GUI */
    QWidget* loadApp(QMenuBar *menuBar);
    Connection m_con; /**< @brief Default connection */
    stackedWContainer *currentPage;
    QMap<int,QString> m_numberFam;
    QMap<QString,stackedWContainer*> m_fam;
    QTabWidget *m_main;
};

#endif // MAINWINDOW_H
