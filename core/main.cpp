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

#include <QtGui/QApplication>
#include <QSplashScreen>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPixmap pixmap(":/P4280017.JPG");

    QSplashScreen splash(pixmap);
    splash.showMessage(QObject::tr("Loading..."));

    splash.show();

    app.processEvents();
    MainWindow w;

    splash.finish(&w);

    w.show();

    return app.exec();
}
