#include "helpviewer.h"
#include "helpviewer_p.h"
#include "helpenginewrapper.h"

#include <QtCore/QFileInfo>
#include <QtCore/QStringBuilder>
#include <QtCore/QTemporaryFile>
#include <QtCore/QCoreApplication>
#include <QtGui/QDesktopServices>
#include <QtWidgets/QDialog>

struct ExtensionMap {
    const char *extension;
    const char *mimeType;
} extensionMap[] = {
    { ".bmp", "image/bmp" },
    { ".css", "text/css" },
    { ".gif", "image/gif" },
    { ".html", "text/html" },
    { ".htm", "text/html" },
    { ".ico", "image/x-icon" },
    { ".jpeg", "image/jpeg" },
    { ".jpg", "image/jpeg" },
    { ".js", "application/x-javascript" },
    { ".mng", "video/x-mng" },
    { ".pbm", "image/x-portable-bitmap" },
    { ".pgm", "image/x-portable-graymap" },
    { ".pdf", nullptr },
    { ".png", "image/png" },
    { ".ppm", "image/x-portable-pixmap" },
    { ".rss", "application/rss+xml" },
    { ".svg", "image/svg+xml" },
    { ".svgz", "image/svg+xml" },
    { ".text", "text/plain" },
    { ".tif", "image/tiff" },
    { ".tiff", "image/tiff" },
    { ".txt", "text/plain" },
    { ".xbm", "image/x-xbitmap" },
    { ".xml", "text/xml" },
    { ".xpm", "image/x-xpm" },
    { ".xsl", "text/xsl" },
    { ".xhtml", "application/xhtml+xml" },
    { ".wml", "text/vnd.wap.wml" },
    { ".wmlc", "application/vnd.wap.wmlc" },
    { "about:blank", nullptr },
    { nullptr, nullptr }
};

const QString HelpViewer::AboutBlank =
    QCoreApplication::translate("HelpViewer", "<title>about:blank</title>");

const QString HelpViewer::PageNotFoundMessage =
    QCoreApplication::translate("HelpViewer", "<title>Error 404...</title><div "
                                              "align=\"center\"><br><br><h1>The page could not be found.</h1><br><h3>'%1'"
                                              "</h3></div>");

HelpViewer::HelpViewer(qreal zoom, QWidget *parent)
    : QWebEngineView(parent)
    , d(new HelpViewerPrivate(zoom))
    , m_channel(new QWebChannel(this))
    , m_webOperate(new WebOperate(this))
{
    m_channel->registerObject(QString("myWebOperate"), m_webOperate);
    page()->setWebChannel(m_channel);
}

HelpViewer::~HelpViewer()
{
    delete d;
}

QUrl HelpViewer::source() const
{
    return QWebEngineView::url();
}

void HelpViewer::setSource(const QUrl &url)
{
    if (launchWithExternalApp(url))
        return;

    const QUrl resolvedUrl = HelpEngineWrapper::instance().findFile(url);
    if (!resolvedUrl.isValid()) {
        bool isBlank = url.toString() == QLatin1String("about:blank");
        setHtml(isBlank ? AboutBlank : PageNotFoundMessage.arg(url.toString()));
    } else {
        load(resolvedUrl);
    }
}

bool HelpViewer::isForwardAvailable() const
{
    return page()->history()->canGoForward();
}

bool HelpViewer::isBackwardAvailable() const
{
    return page()->history()->canGoBack();
}

void HelpViewer::home()
{
    setSource(HelpEngineWrapper::instance().homePage());
}

void HelpViewer::zoomIn()
{
    setZoomFactor(zoomFactor() + 0.1);
}

void HelpViewer::zoomOut()
{
    setZoomFactor(zoomFactor() - 0.1);
}

void HelpViewer::resetZoom()
{
    setZoomFactor(1.0);
}

QString HelpViewer::mimeFromUrl(const QUrl &url)
{
    const QString &path = url.path();
    const int index = path.lastIndexOf(QLatin1Char('.'));
    const QByteArray &ext = path.mid(index).toUtf8().toLower();

    const ExtensionMap *e = extensionMap;
    while (e->extension) {
        if (ext == e->extension)
            return QLatin1String(e->mimeType);
        ++e;
    }

    // 通用二进制数据流类型
    return QLatin1String("application/octet-stream");
}

bool HelpViewer::isLocalUrl(const QUrl &url)
{
    const QString &scheme = url.scheme();
    return scheme.isEmpty()
           || scheme == QLatin1String("file")
           || scheme == QLatin1String("qrc")
           || scheme == QLatin1String("data")
           || scheme == QLatin1String("qthelp")
           || scheme == QLatin1String("about");
}

bool HelpViewer::canOpenPage(const QString &path)
{
    return !mimeFromUrl(QUrl::fromLocalFile(path)).isEmpty();
}

bool HelpViewer::launchWithExternalApp(const QUrl &url)
{
    if (isLocalUrl(url)) {
        const HelpEngineWrapper &helpEngine = HelpEngineWrapper::instance();
        const QUrl &resolvedUrl = helpEngine.findFile(url);
        if (!resolvedUrl.isValid())
            return false;

        const QString& path = resolvedUrl.toLocalFile();
        if (!canOpenPage(path)) {
            QTemporaryFile tmpTmpFile;
            if (!tmpTmpFile.open())
                return false;

            const QString &extension = QFileInfo(path).completeSuffix();
            QFile actualTmpFile(tmpTmpFile.fileName() % QLatin1String(".")
                                % extension);
            if (!actualTmpFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
                return false;

            actualTmpFile.write(helpEngine.fileData(resolvedUrl));
            actualTmpFile.close();
            return QDesktopServices::openUrl(QUrl::fromLocalFile(actualTmpFile.fileName()));
        }
        return false;
    }
    return QDesktopServices::openUrl(url);
}

void HelpViewer::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = page()->createStandardContextMenu();
    QList<QAction*> acts = menu->actions();
    for (auto &act : acts) {
        if (act->text().contains("Save page")
            || act->text().contains("View page source")) {
            menu->removeAction(act);
        }
    }

    menu->exec(event->globalPos());
    delete menu;
}

WebOperate::WebOperate(QObject *parent)
    : QObject(parent)
{}

void WebOperate::bmFunc(const QString &message)
{
    // TODO 测试使用
    QDialog dialog;
    auto ret = dialog.exec();
}
