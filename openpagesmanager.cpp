#include "openpagesmanager.h"
#include "centralwidget.h"
#include "helpenginewrapper.h"
#include "helpviewer.h"
#include "openpagesmodel.h"
#include "openpageswidget.h"
#include "collectionconfiguration.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QAbstractItemView>

OpenPagesManager *OpenPagesManager::m_instance = nullptr;

OpenPagesManager *OpenPagesManager::createInstance(QObject *parent, const QUrl &cmdLineUrl)
{
    Q_ASSERT(!m_instance);
    m_instance = new OpenPagesManager(parent, cmdLineUrl);
    return m_instance;
}

OpenPagesManager *OpenPagesManager::instance()
{
    Q_ASSERT(m_instance);
    return m_instance;
}

OpenPagesManager::OpenPagesManager(QObject *parent, const QUrl &cmdLineUrl)
    : QObject(parent)
    , m_model(new OpenPagesModel(this))
{
    m_openPagesWidget = new OpenPagesWidget(m_model);
    m_openPagesWidget->setFrameStyle(QFrame::NoFrame);
    connect(m_openPagesWidget, &OpenPagesWidget::setCurrentPage,
            this, QOverload<const QModelIndex &>::of(&OpenPagesManager::setCurrentPage));
    connect(m_openPagesWidget, &OpenPagesWidget::closePage,
            this, QOverload<const QModelIndex &>::of(&OpenPagesManager::closePage));
    connect(m_openPagesWidget, &OpenPagesWidget::closePagesExcept,
            this, &OpenPagesManager::closePagesExcept);

    setupInitialPages(cmdLineUrl);
}

OpenPagesManager::~OpenPagesManager()
{
    m_instance = nullptr;
}

int OpenPagesManager::pageCount() const
{
    return m_model->rowCount();
}

void OpenPagesManager::setupInitialPages(const QUrl &cmdLineUrl)
{
    if (cmdLineUrl.isValid()) {
        createPage(cmdLineUrl);
        return;
    }

    HelpEngineWrapper &helpEngine = HelpEngineWrapper::instance();
    int initialPage = 0;
    switch (helpEngine.startOption()) {
    case ShowHomePage:
        m_model->addPage(helpEngine.homePage());
        break;
    case ShowBlankPage:
        m_model->addPage(QUrl(QLatin1String("about:blank")));
        break;
    case ShowLastPages: {
        const QStringList &lastShownPageList = helpEngine.lastShownPages();
        const int pageCount = lastShownPageList.count();
        if (pageCount == 0) {
            m_model->addPage(QUrl(QLatin1String("about:blank")));
        } else {
            QStringList zoomFactors = helpEngine.lastZoomFactors();
            while (zoomFactors.count() < pageCount)
                zoomFactors.append(CollectionConfiguration::DefaultZoomFactor);
            initialPage = helpEngine.lastTabPage();
            if (initialPage >= pageCount) {
                qWarning("Initial page set to %d, maximum possible value is %d",
                         initialPage, pageCount - 1);
                initialPage = 0;
            }
            for (int curPage = 0; curPage < pageCount; ++curPage) {
                const QString &curFile = lastShownPageList.at(curPage);
                if (helpEngine.findFile(curFile).isValid()
                    || curFile == QLatin1String("about:blank")) {
                    m_model->addPage(curFile, zoomFactors.at(curPage).toFloat());
                } else if (curPage <= initialPage && initialPage > 0)
                    --initialPage;
            }
        }
        break;
    }
    default:
        Q_ASSERT(0);
    }

    if (m_model->rowCount() == 0)
        m_model->addPage(helpEngine.homePage());
    for (int i = 0; i < m_model->rowCount(); ++i)
        CentralWidget::instance()->addPage(m_model->pageAt(i));
    setCurrentPage((initialPage >= m_model->rowCount())
                       ? m_model->rowCount() - 1 : initialPage);
}

HelpViewer *OpenPagesManager::createBlankPage()
{
    return createPage(QUrl(QLatin1String("about:blank")));
}

HelpViewer *OpenPagesManager::createPage(const QUrl &url, bool fromSearch)
{
    if (HelpViewer::launchWithExternalApp(url))
        return nullptr;

    m_model->addPage(url);
    const int index = m_model->rowCount() - 1;
    HelpViewer * const page = m_model->pageAt(index);
    CentralWidget::instance()->addPage(page, fromSearch);
    setCurrentPage(index);

    emit pageAdded(index);
    return page;
}

void OpenPagesManager::closePage(HelpViewer *viewer)
{
    for (int i = 0; i < m_model->rowCount(); ++i) {
        if (m_model->pageAt(i) == viewer) {
            removePage(i);
            break;
        }
    }
}

void OpenPagesManager::closePagesExcept(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    int i = 0;
    HelpViewer *viewer = m_model->pageAt(index.row());
    while (m_model->rowCount() > 1) {
        if (m_model->pageAt(i) != viewer)
            removePage(i);
        else
            ++i;
    }
}

void OpenPagesManager::closePage(const QModelIndex &index)
{
    if (index.isValid())
        removePage(index.row());
}

void OpenPagesManager::setCurrentPage(const QModelIndex &index)
{
    if (index.isValid())
        setCurrentPage(index.row());
}

void OpenPagesManager::setCurrentPage(int index)
{
    setCurrentPage(m_model->pageAt(index));
}

QAbstractItemView *OpenPagesManager::openPagesWidget() const
{
    return m_openPagesWidget;
}

void OpenPagesManager::setCurrentPage(HelpViewer *page)
{
    CentralWidget::instance()->setCurrentPage(page);
    m_openPagesWidget->selectCurrentPage();
}

void OpenPagesManager::closeCurrentPage()
{
    Q_ASSERT(m_model->rowCount() > 1);
    const QModelIndexList selectedIndexes
        = m_openPagesWidget->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty())
        return;
    Q_ASSERT(selectedIndexes.count() == 1);
    removePage(selectedIndexes.first().row());
}

void OpenPagesManager::removePage(int index)
{
    CentralWidget::instance()->removePage(index);
    m_model->removePage(index);
    m_openPagesWidget->selectCurrentPage();

    emit pageClosed();
}

void OpenPagesManager::nextPage()
{
    nextOrPreviousPage(1);
}

void OpenPagesManager::previousPage()
{
    nextOrPreviousPage(-1);
}

void OpenPagesManager::nextOrPreviousPage(int offset)
{
    setCurrentPage((CentralWidget::instance()->currentIndex() + offset
                    + m_model->rowCount()) % m_model->rowCount());
}
