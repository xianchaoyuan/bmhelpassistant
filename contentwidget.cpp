#include "contentwidget.h"
#include "helpenginewrapper.h"
#include "centralwidget.h"
#include "openpagesmanager.h"
#include "helpviewer.h"

#include <QtWidgets/QMenu>
#include <QtWidgets/QLayout>
#include <QtGui/QMouseEvent>
#include <QtHelp/QHelpContentWidget>

ContentWidget::ContentWidget(QWidget *parent)
    : QWidget(parent)
    , m_contentWidget(HelpEngineWrapper::instance().contentWidget())
{
    m_contentWidget->viewport()->installEventFilter(this);
    m_contentWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 0);
    layout->addWidget(m_contentWidget);

    connect(m_contentWidget, &QWidget::customContextMenuRequested,
            this, &ContentWidget::showContextMenu);
    connect(m_contentWidget, &QHelpContentWidget::linkActivated,
            this, &ContentWidget::linkActivated);
}

bool ContentWidget::syncToContent(const QUrl &url)
{
    QModelIndex idx = m_contentWidget->indexOf(url);
    if (!idx.isValid())
        return false;
    m_contentWidget->setCurrentIndex(idx);
    return true;
}

void ContentWidget::showContextMenu(const QPoint &pos)
{
    if (!m_contentWidget->indexAt(pos).isValid())
        return;

    QHelpContentModel *contentModel =
        qobject_cast<QHelpContentModel *>(m_contentWidget->model());
    QHelpContentItem *itm =
        contentModel->contentItemAt(m_contentWidget->currentIndex());

    QMenu menu;
    QAction *curTab = menu.addAction(tr("Open Link"));
    QAction *newTab = menu.addAction(tr("Open Link in New Tab"));
    menu.move(m_contentWidget->mapToGlobal(pos));

    QAction *action = menu.exec();
    if (curTab == action) {
        emit linkActivated(itm->url());
    } else if (newTab == action) {
        OpenPagesManager::instance()->createPage(itm->url());
    }
}

void ContentWidget::itemClicked(const QModelIndex &index)
{
    QHelpContentModel *contentModel =
        qobject_cast<QHelpContentModel *>(m_contentWidget->model());

    if (contentModel) {
        if (auto *itm = contentModel->contentItemAt(index)) {
            const QUrl &url = itm->url();
            if (url != CentralWidget::instance()->currentSource())
                emit linkActivated(url);
        }
    }
}

bool ContentWidget::eventFilter(QObject *o, QEvent *e)
{
    if (m_contentWidget && o == m_contentWidget->viewport()
        && e->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *me = static_cast<QMouseEvent*>(e);
        const QModelIndex &index = m_contentWidget->indexAt(me->pos());
        if (!index.isValid())
            return QWidget::eventFilter(o, e);

        const Qt::MouseButton button = me->button();
        QItemSelectionModel *sm = m_contentWidget->selectionModel();
        if (sm->isSelected(index)) {
            if ((button == Qt::LeftButton && (me->modifiers() & Qt::ControlModifier))
                || (button == Qt::MiddleButton)) {
                QHelpContentModel *contentModel =
                    qobject_cast<QHelpContentModel *>(m_contentWidget->model());
                if (contentModel) {
                    QHelpContentItem *itm = contentModel->contentItemAt(index);
                    if (itm && HelpViewer::canOpenPage(itm->url().path()))
                        OpenPagesManager::instance()->createPage(itm->url());
                }
            } else if (button == Qt::LeftButton) {
                itemClicked(index);
            }
        }
    }
    return QWidget::eventFilter(o, e);
}
