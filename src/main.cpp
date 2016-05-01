/***********************************************************************
**
** Sturdy - note-taking app
** Copyright (C) 2016 Vladislav Tronko <innermous@gmail.com>
**
** Sturdy is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Sturdy. If not, see <http://www.gnu.org/licenses/>.
**
***********************************************************************/

#include "core/application.h"

#include <QApplication>
#include <QTimer>

int main(int argc, char* argv[]) {
    QCoreApplication::setApplicationName(QStringLiteral("Sturdy"));
    QCoreApplication::setApplicationVersion(STURDY_VERSION);

    QScopedPointer<QApplication> qapp(new QApplication(argc, argv));
    QScopedPointer<Core::Application> app(new Core::Application);

    QTimer::singleShot(0, app.data(), SLOT(run()));

    return qapp->exec();
}
