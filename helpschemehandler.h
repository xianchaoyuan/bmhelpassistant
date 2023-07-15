#ifndef HELPSCHEMEHANDLER_H
#define HELPSCHEMEHANDLER_H

#include <QtCore/QBuffer>
#include <QtCore/QFileInfo>
#include <QtHelp/QHelpEngine>
#include <QtWebEngineWidgets/QWebEngineProfile>
#include <QtWebEngineCore/QWebEngineUrlSchemeHandler>
#include <QtWebEngineCore/QWebEngineUrlRequestJob>
#include <QtWebEngineCore/QWebEngineUrlScheme>

#include "helpenginewrapper.h"

/**
 * @brief 自定义处理
 */
class HelpSchemeHandler : public QWebEngineUrlSchemeHandler
{
    Q_OBJECT

public:
    HelpSchemeHandler()
    {
        QWebEngineUrlScheme::registerScheme(QWebEngineUrlScheme("qthelp"));
    }

    void install()
    {
        QWebEngineProfile::defaultProfile()->installUrlSchemeHandler("qthelp", this);
    }

    virtual void requestStarted(QWebEngineUrlRequestJob *job) override
    {
        QUrl url = job->requestUrl();
        QByteArray *data = new QByteArray;
        *data = HelpEngineWrapper::instance().fileData(url);
        auto buffer = new QBuffer(data);
        if (url.scheme() == "qthelp") {
            QByteArray contentType;
            QString suffix = QFileInfo(url.path()).suffix();
            if (suffix == "css") {
                contentType = "text/css";
            } else if (suffix == "html") {
                contentType = "text/html";
            } else if (suffix == "png") {
                contentType = "image/png";
            }
            job->reply(contentType, buffer);
        }
    }
};

#endif // HELPSCHEMEHANDLER_H
