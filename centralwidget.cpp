#include "centralwidget.h"

#include <QtWidgets/QVBoxLayout>

namespace {
    CentralWidget *staticCentralWidget = nullptr;
}

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
    , m_stackedWidget(new QStackedWidget(this))
{
    staticCentralWidget = this;

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    vboxLayout->setContentsMargins(QMargins());
    vboxLayout->setSpacing(0);
    vboxLayout->addWidget(m_stackedWidget);
}

CentralWidget *CentralWidget::instance()
{
    return staticCentralWidget;
}

bool CentralWidget::isForwardAvailable() const
{
    // TODO 添加判断
    return true;
}

bool CentralWidget::isBackwardAvailable() const
{
    // TODO 添加判断
    return true;
}

void CentralWidget::backward()
{

}

void CentralWidget::forward()
{

}

void CentralWidget::home()
{

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

}

void CentralWidget::zoomOut()
{

}

void CentralWidget::resetZoom()
{

}
