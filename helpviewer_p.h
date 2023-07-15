#ifndef HELPVIEWER_P_H
#define HELPVIEWER_P_H

#include "helpviewer.h"
#include "centralwidget.h"
#include "openpagesmanager.h"

#include <QtCore/QObject>

/**
 * @brief 阅读器私有数据
 */
class HelpViewer::HelpViewerPrivate : public QObject
{
    Q_OBJECT

public:
    HelpViewerPrivate(int zoom)
        : zoomFactor(zoom) {}

public slots:
    void openLink()
    {
        doOpenLink(false);
    }

    void openLinkInNewPage()
    {
        doOpenLink(true);
    }

public:
    QString lastAnchor;
    qreal zoomFactor;

private:
    void doOpenLink(bool newPage)
    {
        if (lastAnchor.isEmpty())
            return;
        if (newPage) {
            OpenPagesManager::instance()->createPage(lastAnchor);
        } else {
            CentralWidget::instance()->setSource(lastAnchor);
        }
        lastAnchor.clear();
    }
};

#endif // HELPVIEWER_P_H
