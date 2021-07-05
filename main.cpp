/**
 * Copyright 2020-2021 Dimitris Psathas <dimitrisinbox@gmail.com>
 *
 * This file is part of Chores.
 *
 * Chores is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License  as  published by  the  Free Software
 * Foundation,  either version 3 of the License,  or (at your option)  any later
 * version.
 *
 * Chores is distributed in the hope that it will be useful,  but  WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the  GNU General Public License  for more details.
 *
 * You should have received a copy of the  GNU General Public License along with
 * Chores. If not, see <http://www.gnu.org/licenses/>.
 */


#include "global.h"
#include "mainwindow.h"
#include "runguard.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    RunGuard guard("a_key");
    if (!guard.tryToRun())
        return 0;
    QApplication a(argc, argv);
    QApplication::setOrganizationName("DP Software");
    QApplication::setApplicationName(APPNAME);
    QApplication::setApplicationVersion(VERSION);

    MainWindow w;
    w.show();
    return a.exec();
}
