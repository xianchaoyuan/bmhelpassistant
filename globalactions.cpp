#include "globalactions.h"
#include "centralwidget.h"

GlobalActions *GlobalActions::instance(QObject *parent)
{
    static GlobalActions *self = new GlobalActions(parent);
    return self;
}

void GlobalActions::updateActions()
{
    CentralWidget *centralWidget = CentralWidget::instance();
    m_backwardAction->setEnabled(centralWidget->isBackwardAvailable());
    m_forwardAction->setEnabled(centralWidget->isForwardAvailable());
}

GlobalActions::GlobalActions(QObject *parent)
    : QObject(parent)
{
    QString resourcePath = QLatin1String(":/bmhelpassistant/images/");

    CentralWidget *centralWidget = CentralWidget::instance();

    m_backwardAction = new QAction(tr("&Back"), parent);
    m_backwardAction->setEnabled(false);
    m_backwardAction->setShortcuts(QKeySequence::Back);
    m_backwardAction->setIcon(QIcon(resourcePath + QLatin1String("/backward.png")));
    connect(m_backwardAction, &QAction::triggered, centralWidget, &CentralWidget::backward);
    m_actionList << m_backwardAction;

    m_forwardAction = new QAction(tr("&Forward"), parent);
    m_forwardAction->setPriority(QAction::LowPriority);
    m_forwardAction->setEnabled(false);
    m_forwardAction->setShortcuts(QKeySequence::Forward);
    m_forwardAction->setIcon(QIcon(resourcePath + QLatin1String("/forward.png")));
    connect(m_forwardAction, &QAction::triggered, centralWidget, &CentralWidget::forward);
    m_actionList << m_forwardAction;

    m_homeAction = new QAction(tr("&Home"), parent);
    m_homeAction->setShortcut(tr("ALT+Home"));
    m_homeAction->setIcon(QIcon(resourcePath + QLatin1String("/home.png")));
    connect(m_homeAction, &QAction::triggered, centralWidget, &CentralWidget::home);
    m_actionList << m_homeAction;

    QAction *separator = new QAction(parent);
    separator->setSeparator(true);
    m_actionList << separator;

    m_zoomInAction = new QAction(tr("Zoom &in"), parent);
    m_zoomInAction->setPriority(QAction::LowPriority);
    m_zoomInAction->setIcon(QIcon(resourcePath + QLatin1String("/zoomin.png")));
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn);
    connect(m_zoomInAction, &QAction::triggered, centralWidget, &CentralWidget::zoomIn);
    m_actionList << m_zoomInAction;

    m_zoomOutAction = new QAction(tr("Zoom &out"), parent);
    m_zoomOutAction->setPriority(QAction::LowPriority);
    m_zoomOutAction->setIcon(QIcon(resourcePath + QLatin1String("/zoomout.png")));
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    connect(m_zoomOutAction, &QAction::triggered, centralWidget, &CentralWidget::zoomOut);
    m_actionList << m_zoomOutAction;

    m_resetZoomAction = new QAction(tr("Normal &Size"), parent);
    m_resetZoomAction->setPriority(QAction::LowPriority);
    m_resetZoomAction->setIcon(QIcon(resourcePath + QLatin1String("/resetzoom.png")));
    m_resetZoomAction->setShortcut(tr("Ctrl+0"));
    connect(m_resetZoomAction, &QAction::triggered, centralWidget, &CentralWidget::resetZoom);
    m_actionList << m_resetZoomAction;

    separator = new QAction(parent);
    separator->setSeparator(true);
    m_actionList << separator;

    m_printAction = new QAction(tr("&Print..."), parent);
    m_printAction->setPriority(QAction::LowPriority);
    m_printAction->setIcon(QIcon(resourcePath + QLatin1String("/print.png")));
    m_printAction->setShortcut(QKeySequence::Print);
    connect(m_printAction, &QAction::triggered, centralWidget, &CentralWidget::print);
    m_actionList << m_printAction;

    m_findAction = new QAction(tr("&Find in Text..."), parent);
    m_findAction->setIconText(tr("&Find"));
    m_findAction->setIcon(QIcon(resourcePath + QLatin1String("/find.png")));
    m_findAction->setShortcuts(QKeySequence::Find);
    connect(m_findAction, &QAction::triggered, centralWidget, &CentralWidget::showFindWidget);
    m_actionList << m_findAction;
}

