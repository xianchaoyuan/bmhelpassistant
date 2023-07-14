#include "mainwindow.h"
#include "centralwidget.h"
#include "globalactions.h"

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    setDockOptions(dockOptions() | AllowNestedDocks);

    m_centralWidget = new CentralWidget(this);
    setCentralWidget(m_centralWidget);

    // 默认标题
    QString defWindowTitle = tr("bmseven");
    setWindowTitle(defWindowTitle);

    setupActions();
    statusBar()->show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::syncContents()
{

}

void MainWindow::showContents()
{

}

void MainWindow::showOpenPages()
{

}

void MainWindow::setupActions()
{
    QString resourcePath = QLatin1String(":/bmhelpassistant/images/");

    QMenu *menu = menuBar()->addMenu(tr("&File"));
    m_newTabAction = menu->addAction(tr("&New Tab"));
    m_newTabAction->setShortcut(QKeySequence::AddTab);

    menu->addSeparator();

    m_pageSetupAction = menu->addAction(tr("&Page Setup..."),
                                        m_centralWidget, &CentralWidget::pageSetup);
    m_printPreviewAction = menu->addAction(tr("&Print Preview..."),
                                           m_centralWidget, &CentralWidget::printPreview);

    GlobalActions *globalActions = GlobalActions::instance(this);
    menu->addAction(globalActions->printAction());
    menu->addSeparator();

    QIcon appExitIcon = QIcon::fromTheme("application-exit");
    QAction *tmp;
    tmp = menu->addAction(appExitIcon, tr("&Exit"), this, &QWidget::close);
    tmp->setShortcut(QKeySequence(tr("CTRL+Q")));
    tmp->setMenuRole(QAction::QuitRole);

    menu = menuBar()->addMenu(tr("&Edit"));

    menu = menuBar()->addMenu(tr("&View"));
    menu->addAction(globalActions->zoomInAction());
    menu->addAction(globalActions->zoomOutAction());
    menu->addAction(globalActions->resetZoomAction());

    menu->addSeparator();
    menu->addAction(tr("Contents"),
                          this, &MainWindow::showContents, QKeySequence(tr("ALT+C")));
    menu->addAction(tr("Open Pages"),
                          this, &MainWindow::showOpenPages, QKeySequence(tr("ALT+P")));

    menu = menuBar()->addMenu(tr("&Go"));
    menu->addAction(globalActions->homeAction());
    menu->addAction(globalActions->backwardAction());
    menu->addAction(globalActions->forwardAction());

    m_syncAction = menu->addAction(tr("Sync with Table of Contents"),
                                   this, &MainWindow::syncContents);
    m_syncAction->setIconText(tr("Sync"));
    m_syncAction->setIcon(QIcon(resourcePath + QLatin1String("/synctoc.png")));

    menu->addSeparator();

    tmp = menu->addAction(tr("Next Page"));
    tmp->setShortcuts(QList<QKeySequence>() << QKeySequence(tr("Ctrl+Alt+Right"))
                                            << QKeySequence(Qt::CTRL | Qt::Key_PageDown));

    tmp = menu->addAction(tr("Previous Page"));
    tmp->setShortcuts(QList<QKeySequence>() << QKeySequence(tr("Ctrl+Alt+Left"))
                                            << QKeySequence(Qt::CTRL | Qt::Key_PageUp));

    QToolBar *navigationBar = addToolBar(tr("Navigation Toolbar"));
    navigationBar->setObjectName(QLatin1String("NavigationToolBar"));
    navigationBar->addAction(globalActions->backwardAction());
    navigationBar->addAction(globalActions->forwardAction());
    navigationBar->addAction(globalActions->homeAction());
    navigationBar->addAction(m_syncAction);
    navigationBar->addSeparator();
    navigationBar->addAction(globalActions->printAction());
    navigationBar->addSeparator();
    navigationBar->addAction(globalActions->zoomInAction());
    navigationBar->addAction(globalActions->zoomOutAction());
    navigationBar->addAction(globalActions->resetZoomAction());
}

