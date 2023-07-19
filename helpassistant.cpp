#include "helpassistant.h"
#include "contentwidget.h"
#include "centralwidget.h"
#include "globalactions.h"
#include "openpagesmanager.h"
#include "helpenginewrapper.h"

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QAbstractItemView>
#include <QtGui/QGuiApplication>

HelpAssistant::HelpAssistant(const QString &collectionFile, QWidget *parent)
    : QMainWindow(parent)
{
    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    setDockOptions(dockOptions() | AllowNestedDocks);

    HelpEngineWrapper::instance(collectionFile);

    m_centralWidget = new CentralWidget(this);
    setCentralWidget(m_centralWidget);

    m_contentWidget = new ContentWidget(this);
    QDockWidget *contentDock = new QDockWidget(tr("Contents"), this);
    contentDock->setObjectName(QLatin1String("ContentWidget"));
    contentDock->setWidget(m_contentWidget);
    addDockWidget(Qt::LeftDockWidgetArea, contentDock);
    connect(m_contentWidget, &ContentWidget::linkActivated,
            m_centralWidget, &CentralWidget::setSource);

    QDockWidget *openPagesDock = new QDockWidget(tr("Open Pages"), this);
    openPagesDock->setObjectName(QLatin1String("Open Pages"));
    OpenPagesManager *openPagesManager
        = OpenPagesManager::createInstance(this, QUrl{});
    openPagesDock->setWidget(openPagesManager->openPagesWidget());
    addDockWidget(Qt::LeftDockWidgetArea, openPagesDock);

    // 默认标题
    QString defWindowTitle = tr("bmseven");
    setWindowTitle(defWindowTitle);

    setupActions();
    statusBar()->show();
    m_centralWidget->connectTabBar();
}

HelpAssistant::~HelpAssistant()
{
}

void HelpAssistant::syncContents()
{
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    const QUrl url = m_centralWidget->currentSource();
    showContents();
    if (!m_contentWidget->syncToContent(url))
        statusBar()->showMessage(
            tr("Could not find the associated content item."), 3000);
    qApp->restoreOverrideCursor();
}

void HelpAssistant::showContents()
{
    activateDockWidget(m_contentWidget);
}

void HelpAssistant::activateDockWidget(QWidget *w)
{
    w->parentWidget()->show();
    w->parentWidget()->raise();
    w->setFocus();
}

void HelpAssistant::handlePageCountChanged()
{
    m_closeTabAction->setEnabled(OpenPagesManager::instance()->pageCount() > 1);
}

void HelpAssistant::setupActions()
{
    QString resourcePath = QLatin1String(":/bmhelpassistant/images/");

    QMenu *menu = menuBar()->addMenu(tr("&File"));
    OpenPagesManager * const openPages = OpenPagesManager::instance();
    m_newTabAction = menu->addAction(tr("&New Tab"),
                                     openPages, &OpenPagesManager::createBlankPage);
    m_newTabAction->setShortcut(QKeySequence::AddTab);
    m_closeTabAction = menu->addAction(tr("&Close Tab"),
                                       openPages, &OpenPagesManager::closeCurrentPage);
    m_closeTabAction->setShortcuts(QKeySequence::Close);
    m_closeTabAction->setEnabled(openPages->pageCount() > 1);
    connect(openPages, &OpenPagesManager::pageClosed,
            this, &HelpAssistant::handlePageCountChanged);
    connect(openPages, &OpenPagesManager::pageAdded,
            this, &HelpAssistant::handlePageCountChanged);

    menu->addSeparator();

    m_pageSetupAction = menu->addAction(tr("&Page Setup..."),
                                        m_centralWidget, &CentralWidget::pageSetup);

    GlobalActions *globalActions = GlobalActions::instance(this);
    menu->addAction(globalActions->printAction());
    menu->addSeparator();

    QIcon appExitIcon = QIcon::fromTheme("application-exit");
    QAction *tmp;
    tmp = menu->addAction(appExitIcon, tr("&Exit"), this, &QWidget::close);
    tmp->setShortcut(QKeySequence(tr("CTRL+Q")));
    tmp->setMenuRole(QAction::QuitRole);

    menu = menuBar()->addMenu(tr("&Edit"));
    menu->addAction(globalActions->findAction());

    QAction *findNextAction = menu->addAction(tr("Find &Next"),
                                              m_centralWidget, &CentralWidget::findNext);
    findNextAction->setShortcuts(QKeySequence::FindNext);

    QAction *findPreviousAction = menu->addAction(tr("Find &Previous"),
                                                  m_centralWidget, &CentralWidget::findPrevious);
    findPreviousAction->setShortcuts(QKeySequence::FindPrevious);

    menu = menuBar()->addMenu(tr("&View"));
    menu->addAction(globalActions->zoomInAction());
    menu->addAction(globalActions->zoomOutAction());
    menu->addAction(globalActions->resetZoomAction());

    menu->addSeparator();
    menu->addAction(tr("Contents"),
                          this, &HelpAssistant::showContents, QKeySequence(tr("ALT+C")));

    menu = menuBar()->addMenu(tr("&Go"));
    menu->addAction(globalActions->homeAction());
    menu->addAction(globalActions->backwardAction());
    menu->addAction(globalActions->forwardAction());

    m_syncAction = menu->addAction(tr("Sync with Table of Contents"),
                                   this, &HelpAssistant::syncContents);
    m_syncAction->setIconText(tr("Sync"));
    m_syncAction->setIcon(QIcon(resourcePath + QLatin1String("/synctoc.png")));

    menu->addSeparator();

    tmp = menu->addAction(tr("Next Page"), openPages, &OpenPagesManager::nextPage);
    tmp->setShortcuts(QList<QKeySequence>() << QKeySequence(tr("Ctrl+Alt+Right"))
                                            << QKeySequence(Qt::CTRL | Qt::Key_PageDown));

    tmp = menu->addAction(tr("Previous Page"), openPages, &OpenPagesManager::previousPage);
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

    connect(m_centralWidget, &CentralWidget::currentViewerChanged,
            globalActions, &GlobalActions::updateActions);
    connect(m_centralWidget, &CentralWidget::currentViewerLoadFinished,
            globalActions, &GlobalActions::updateActions);
}

