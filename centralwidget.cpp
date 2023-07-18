#include "centralwidget.h"
#include "findwidget.h"
#include "helpviewer.h"
#include "helpenginewrapper.h"
#include "openpagesmanager.h"

#include <QtCore/QUrl>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QVBoxLayout>
#include <QtWebEngineWidgets/QWebEngineHistory>

namespace {
CentralWidget *staticCentralWidget = nullptr;
}

TabBar::TabBar(QWidget *parent)
    : QTabBar(parent)
{
    setMovable(true);
    setShape(QTabBar::RoundedNorth);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred,
                              QSizePolicy::TabWidget));
    connect(this, &QTabBar::currentChanged, this, &TabBar::slotCurrentChanged);
    connect(this, &QTabBar::tabCloseRequested, this, &TabBar::slotTabCloseRequested);
    connect(this, &QWidget::customContextMenuRequested, this, &TabBar::slotCustomContextMenuRequested);
}

TabBar::~TabBar()
{}

int TabBar::addNewTab(const QString &title)
{
    const int index = addTab(title);
    setTabsClosable(count() > 1);
    return index;
}

void TabBar::removeTabAt(HelpViewer *viewer)
{
    for (int i = 0; i < count(); ++i) {
        HelpViewer *data = tabData(i).value<HelpViewer *>();
        if (data == viewer) {
            removeTab(i);
            break;
        }
    }
    setTabsClosable(count() > 1);
}

void TabBar::setCurrent(HelpViewer *viewer)
{
    for (int i = 0; i < count(); ++i) {
        HelpViewer *data = tabData(i).value<HelpViewer *>();
        if (data == viewer) {
            setCurrentIndex(i);
            break;
        }
    }
}

void TabBar::titleChanged(const QString &title)
{
    // 屏蔽url导致的title变化
    if (title.startsWith("qthelp://")) {
        return;
    }

    for (int i = 0; i < count(); ++i) {
        HelpViewer *data = tabData(i).value<HelpViewer *>();
        QString title = data->title();
        title.replace(QLatin1Char('&'), QLatin1String("&&"));
        setTabText(i, title.isEmpty() ? tr("(Untitled)") : title);
    }
}

void TabBar::slotCurrentChanged(int index)
{
    emit currentTabChanged(tabData(index).value<HelpViewer *>());
}

void TabBar::slotTabCloseRequested(int index)
{
    OpenPagesManager::instance()->closePage(tabData(index).value<HelpViewer *>());
}

void TabBar::slotCustomContextMenuRequested(const QPoint &pos)
{
    const int tab = tabAt(pos);
    if (tab < 0)
        return;

    QMenu menu(QString(), this);
    menu.addAction(tr("New &Tab"), OpenPagesManager::instance(),
                   &OpenPagesManager::createBlankPage);

    const bool enableAction = count() > 1;
    QAction *closePage = menu.addAction(tr("&Close Tab"));
    closePage->setEnabled(enableAction);

    QAction *closePages = menu.addAction(tr("Close Other Tabs"));
    closePages->setEnabled(enableAction);

    menu.addSeparator();

    QAction *pickedAction = menu.exec(mapToGlobal(pos));
    if (pickedAction == closePage)
        slotTabCloseRequested(tab);
    else if (pickedAction == closePages) {
        for (int i = count() - 1; i >= 0; --i) {
            if (i != tab)
                slotTabCloseRequested(i);
        }
    }
}

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
    , m_tabBar(new TabBar(this))
    , m_stackedWidget(new QStackedWidget(this))
    , m_findWidget(new FindWidget(this))
{
    staticCentralWidget = this;

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    vboxLayout->setContentsMargins(QMargins());
    vboxLayout->setSpacing(0);
    vboxLayout->addWidget(m_tabBar);
    m_tabBar->setVisible(true);
    vboxLayout->addWidget(m_stackedWidget);
    vboxLayout->addWidget(m_findWidget);
    m_findWidget->hide();

    connect(m_findWidget, &FindWidget::clearFind, this, &CentralWidget::clearFind);
    connect(m_findWidget, &FindWidget::findNext, this, &CentralWidget::findNext);
    connect(m_findWidget, &FindWidget::findPrevious, this, &CentralWidget::findPrevious);
    connect(m_findWidget, &FindWidget::find, this, &CentralWidget::find);
}

CentralWidget::~CentralWidget()
{
}

CentralWidget *CentralWidget::instance()
{
    return staticCentralWidget;
}

QUrl CentralWidget::currentSource() const
{
    return currentHelpViewer()->source();
}

QString CentralWidget::currentTitle() const
{
    return currentHelpViewer()->title();
}

HelpViewer *CentralWidget::viewerAt(int index) const
{
    return static_cast<HelpViewer *>(m_stackedWidget->widget(index));
}

HelpViewer *CentralWidget::currentHelpViewer() const
{
    return static_cast<HelpViewer *>(m_stackedWidget->currentWidget());
}

int CentralWidget::currentIndex() const
{
    return m_stackedWidget->currentIndex();
}

void CentralWidget::connectTabBar()
{
    connect(m_tabBar, &TabBar::currentTabChanged, OpenPagesManager::instance(),
            QOverload<HelpViewer *>::of(&OpenPagesManager::setCurrentPage));
}

bool CentralWidget::isForwardAvailable() const
{
    return currentHelpViewer()->isForwardAvailable();
}

bool CentralWidget::isBackwardAvailable() const
{
    return currentHelpViewer()->isBackwardAvailable();
}

void CentralWidget::backward()
{
    currentHelpViewer()->back();
}

void CentralWidget::forward()
{
    currentHelpViewer()->forward();
}

void CentralWidget::home()
{
    currentHelpViewer()->home();
}

void CentralWidget::print()
{

}

void CentralWidget::pageSetup()
{

}

void CentralWidget::printPreview()
{

}

void CentralWidget::zoomIn()
{
    currentHelpViewer()->zoomIn();
}

void CentralWidget::zoomOut()
{
    currentHelpViewer()->zoomOut();
}

void CentralWidget::resetZoom()
{
    currentHelpViewer()->resetZoom();
}

void CentralWidget::setSource(const QUrl &url)
{
    HelpViewer *viewer = currentHelpViewer();
    viewer->setSource(url);
    viewer->setFocus(Qt::OtherFocusReason);
}

void CentralWidget::clearFind()
{
    HelpViewer *viewer = currentHelpViewer();
    viewer->findText(QString(""));
}

void CentralWidget::findNext()
{
    find(m_findWidget->text(), true);
}

void CentralWidget::findPrevious()
{
    find(m_findWidget->text(), false);
}

void CentralWidget::find(const QString &text, bool forward)
{
    if (HelpViewer *viewer = currentHelpViewer()) {
        QWebEnginePage::FindFlags flags;
        if (!forward)
            flags |= QWebEnginePage::FindBackward;
        if (m_findWidget->caseSensitive())
            flags |= QWebEnginePage::FindCaseSensitively;
        viewer->findText(text, flags);
    }

    if (!m_findWidget->isVisible())
        m_findWidget->show();
}

void CentralWidget::showFindWidget()
{
    m_findWidget->show();
}

void CentralWidget::addPage(HelpViewer *page, bool fromSearch)
{
    Q_UNUSED(fromSearch);

    page->installEventFilter(this);
    page->setFocus(Qt::OtherFocusReason);
    connect(page, &HelpViewer::loadFinished, this,
            &CentralWidget::currentViewerLoadFinished);
    const int index = m_stackedWidget->addWidget(page);
    m_tabBar->setTabData(m_tabBar->addNewTab(page->title()),
                         QVariant::fromValue(viewerAt(index)));
    connect(page, &HelpViewer::titleChanged, m_tabBar, &TabBar::titleChanged);
}

void CentralWidget::removePage(int index)
{
    const bool currentChanged = index == currentIndex();
    m_tabBar->removeTabAt(viewerAt(index));
    m_stackedWidget->removeWidget(m_stackedWidget->widget(index));
    if (currentChanged)
        emit currentViewerChanged();
}

void CentralWidget::setCurrentPage(HelpViewer *page)
{
    m_tabBar->setCurrent(page);
    m_stackedWidget->setCurrentWidget(page);
    emit currentViewerChanged();
}
