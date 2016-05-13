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
#include "core/settings.h"
#include "core/profilemanager.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mvc/notebooksproxymodel.h"
#include "mvc/entriesproxymodel.h"

#include <QItemSelectionModel>
#include <QSqlTableModel>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QMessageBox>
#include <QWebChannel>

MainWindow::MainWindow(Core::Application* app, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_application(app)
    , m_settings(app->settings())
    , m_profileManager(app->profileManager())
    , m_nbProxyModel(new Mvc::NotebooksProxyModel)
    , m_entriesProxyModel(new Mvc::EntriesProxyModel)
    , m_nbSelectionModel(new QItemSelectionModel(m_nbProxyModel.data()))
    , m_entriesSelectionModel(new QItemSelectionModel(m_entriesProxyModel.data()))
{
    ui->setupUi(this);

    ui->lstEntries->setFocus();

    restoreGeometry(m_settings->windowGeometry);
    ui->splitter->restoreState(m_settings->verticalSplitterGeometry);
    ui->selSplitter->restoreState(m_settings->selectionSplitterGeometry);

    // Menu

    // - File

    // -- Switch profile
    updateSwitchProfileMenu();

    // -- Quit
    if (QKeySequence(QKeySequence::Quit).isEmpty())
        ui->actionQuit->setShortcut(QStringLiteral("Ctrl+Q"));
    else
        ui->actionQuit->setShortcut(QKeySequence::Quit);

    connect(ui->actionQuit, &QAction::triggered, qApp, QCoreApplication::quit);

    // - Help

    // -- Submit feedback
    connect(ui->actionFeedback, &QAction::triggered, [this]() {
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://github.com/innermous/sturdy/issues")));
    });

    // -- About Qt
    connect(ui->actionAboutQt, &QAction::triggered, [this]() {
        QMessageBox::aboutQt(this);
    });

    // Selection buttons

    // - Entries remove button
    connect(ui->btnRemoveEntry, &QToolButton::pressed, [this]() {
        m_entriesProxyModel->removeRow(ui->lstEntries->currentIndex().row());
        m_entriesModel->select();
    });

    // - Notebooks remove button
    connect(ui->btnRemoveNotebook, &QToolButton::pressed, [this]() {
        int currentRow = ui->lstNotebooks->currentIndex().row();
        m_nbProxyModel->removeRow(ui->lstNotebooks->currentIndex().row());
        m_nbModel->select();
        m_entriesModel->select();

        // Select next notebook
        ui->lstNotebooks->setCurrentIndex(m_nbModel->index(currentRow, 0));
    });

    // Selection widgets
    ui->lstNotebooks->setModel(m_nbProxyModel.data());
    ui->lstEntries->setModel(m_entriesProxyModel.data());

    connect(ui->lstNotebooks->selectionModel(), &QItemSelectionModel::currentRowChanged,
            m_nbProxyModel.data(), &Mvc::NotebooksProxyModel::setCurrentNotebook);
    connect(ui->lstEntries->selectionModel(), &QItemSelectionModel::currentRowChanged,
            m_entriesProxyModel.data(), &Mvc::EntriesProxyModel::setCurrentEntry);
    connect(m_nbProxyModel.data(), &Mvc::NotebooksProxyModel::notebookChanged,
            m_entriesProxyModel.data(), &Mvc::EntriesProxyModel::changeNotebook);
    connect(m_nbProxyModel.data(), &Mvc::NotebooksProxyModel::notebookChanged,
            this, &MainWindow::changeNotebook);

    // Editor
    connect(m_entriesProxyModel.data(), &Mvc::EntriesProxyModel::entrySelected,
            ui->editor, &EditorWidget::setCurrentEntry);
    connect(m_entriesProxyModel.data(), &Mvc::EntriesProxyModel::entryRemoved,
            ui->editor, &EditorWidget::closeEntry);
    connect(ui->editor, &QPlainTextEdit::textChanged, [this]() {
        m_previewContent.setText(ui->editor->toPlainText());
    });

    PreviewPage* m_previewPage = new PreviewPage(this);
    ui->preview->setPage(m_previewPage);
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), &m_previewContent);
    m_previewPage->setWebChannel(channel);
}

MainWindow::~MainWindow()
{
    m_settings->verticalSplitterGeometry = ui->splitter->saveState();
    m_settings->selectionSplitterGeometry = ui->selSplitter->saveState();
    m_settings->windowGeometry = saveGeometry();

    if (m_nbModel)
        delete m_nbModel;
    if (m_entriesModel)
        delete m_entriesModel;

    delete ui;
}

void MainWindow::updateSwitchProfileMenu()
{
    QMenu* menu = ui->menuSwitchProfile;

    // clear old actions
    qDeleteAll(menu->actions());

    QStringList availableProfiles = m_profileManager->availableProfiles();
    QString currentProfile = m_profileManager->currentProfile();
    for (const auto& profile : availableProfiles) {
        QAction* action = new QAction(profile, menu);
        menu->addAction(action);
        if (profile == m_profileManager->currentProfile())
            action->setEnabled(false);
        else
            connect(action, &QAction::triggered, [=]() {
                requestProfileChange(profile);
            });
    }
}

void MainWindow::requestProfileChange(const QString& profile)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(0,
                                  "",
                                  QStringLiteral("Switch to profile <i>%1</i>?").arg(profile),
                                  QMessageBox::Yes | QMessageBox::Cancel);

    if (reply == QMessageBox::Yes) {
        closeProfile();
        loadProfile(profile);
    }
}

void MainWindow::loadProfile(const QString& profile)
{
    if (!m_profileManager->initProfile(profile)) {
        QMessageBox::critical(0,
                              QStringLiteral("Something went wrong"),
                              QStringLiteral("Profile failed to load"),
                              QMessageBox::Ok
                              );
        updateSwitchProfileMenu();
        return;
    }

    updateSwitchProfileMenu();

    m_nbModel = new QSqlTableModel;
    m_nbProxyModel.data()->setSourceModel(m_nbModel);
    m_nbModel->setTable(QStringLiteral("notebooks"));
    m_nbModel->select();

    m_entriesModel = new QSqlTableModel;
    m_entriesProxyModel.data()->setSourceModel(m_entriesModel);
    m_entriesModel->setTable(QStringLiteral("entries"));
    m_entriesModel->select();
}

void MainWindow::closeProfile()
{
    if (m_nbModel)
        delete m_nbModel;
    if (m_entriesModel)
        delete m_entriesModel;

    m_profileManager->closeProfile();
}

void MainWindow::changeNotebook(int id)
{
    m_entriesModel->setFilter(QStringLiteral("NOTEBOOK_ID = %1").arg(id));
}
