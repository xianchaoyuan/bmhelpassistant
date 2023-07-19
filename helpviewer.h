#ifndef HELPVIEWER_H
#define HELPVIEWER_H

#include <QtGui/QContextMenuEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWebChannel/QWebChannel>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEngineHistory>

/**
 * @brief web操作
 */
class WebOperate : public QObject
{
    Q_OBJECT

public:
    explicit WebOperate(QObject *parent = nullptr);
    ~WebOperate() = default;

public slots:
    void bmFunc(const QString &message);
};

/**
 * @brief 阅读器
 */
class HelpViewer : public QWebEngineView
{
    Q_OBJECT
    Q_DISABLE_COPY(HelpViewer)

    class HelpViewerPrivate;

public:
    HelpViewer(qreal zoom, QWidget *parent = nullptr);
    ~HelpViewer() override;

    QUrl source() const;
    void setSource(const QUrl &url);

    bool isForwardAvailable() const;
    bool isBackwardAvailable() const;

    void home();
    void print(QPrinter *printer);

    void zoomIn();
    void zoomOut();
    void resetZoom();

    static const QString AboutBlank;
    static const QString PageNotFoundMessage;

    static QString mimeFromUrl(const QUrl &url);
    static bool isLocalUrl(const QUrl &url);
    static bool canOpenPage(const QString &url);
    static bool launchWithExternalApp(const QUrl &url);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    HelpViewerPrivate *d;
    QWebChannel *m_channel;
    WebOperate *m_webOperate;
};

Q_DECLARE_METATYPE(HelpViewer *)

#endif // HELPVIEWER_H
