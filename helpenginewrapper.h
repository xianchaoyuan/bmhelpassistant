#ifndef HELPENGINEWRAPPER_H
#define HELPENGINEWRAPPER_H

#include <QtCore/QObject>

class QHelpSearchEngine;
class QHelpContentWidget;
class QHelpEngineCore;

class HelpEngineWrapperPrivate;

enum {
    ShowHomePage = 0,
    ShowBlankPage = 1,
    ShowLastPages = 2
};

/**
 * @brief 帮助引擎包装器
 */
class HelpEngineWrapper : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(HelpEngineWrapper)

public:
    static HelpEngineWrapper &instance(const QString &collectionFile = QString());
    static void removeInstance();

    QHelpSearchEngine *searchEngine() const;
    QHelpContentWidget *contentWidget() const;

    bool setupData();

    QUrl findFile(const QUrl &url) const;
    QByteArray fileData(const QUrl &url) const;

    const QString homePage() const;
    void setHomePage(const QString &page);

    const QString defaultHomePage() const;
    void setDefaultHomePage(const QString &page);

    int lastTabPage() const;
    void setLastTabPage(int lastPage);

    const QStringList lastShownPages() const;
    void setLastShownPages(const QStringList &lastShownPages);

    const QStringList lastZoomFactors() const;
    void setLastZoomFactors(const QStringList &lastZoomFactors);

    int startOption() const;
    void setStartOption(int option);

    QHelpEngineCore *helpEngine() const;

private:
    explicit HelpEngineWrapper(const QString &collectionFile);
    ~HelpEngineWrapper();

    static HelpEngineWrapper *helpEngineWrapper;

    HelpEngineWrapperPrivate *d;
};

#endif // HELPENGINEWRAPPER_H
