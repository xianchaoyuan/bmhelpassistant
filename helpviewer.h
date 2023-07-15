#ifndef HELPVIEWER_H
#define HELPVIEWER_H

#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEngineHistory>

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

    void zoomIn();
    void zoomOut();
    void resetZoom();

    static const QString AboutBlank;
    static const QString PageNotFoundMessage;

    static QString mimeFromUrl(const QUrl &url);
    static bool isLocalUrl(const QUrl &url);
    static bool canOpenPage(const QString &url);
    static bool launchWithExternalApp(const QUrl &url);

private:
    HelpViewerPrivate *d;
};

Q_DECLARE_METATYPE(HelpViewer *)

#endif // HELPVIEWER_H
