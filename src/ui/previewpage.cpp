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

#include "previewpage.h"

#include <QDesktopServices>

Document::Document(QObject* parent)
    : QObject(parent)
{

}

void Document::setText(const QString &text)
{
    if (text == m_text)
        return;
    m_text = text;

    emit textChanged(m_text);
}

PreviewPage::PreviewPage(QObject* parent)
    : QWebEnginePage(parent)
{
    setUrl(QUrl(QStringLiteral("qrc:/editor/editor/index.html")));
}

bool PreviewPage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType, bool isMainFrame)
{
    Q_UNUSED (isMainFrame)

    // Only allow qrc:/index.html.
    if (url.scheme() == QString("qrc"))
        return true;

    QDesktopServices::openUrl(url);
    return false;
}

