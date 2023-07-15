#include "helpenginewrapper.h"
#include "helpenginewrapper_p.h"
#include "collectionconfiguration.h"

#include <QtHelp/QHelpSearchEngine>
#include <QtHelp/QHelpContentWidget>

namespace {
    const QString HomePageKey(QLatin1String("homepage"));
    const QString StartOptionKey(QLatin1String("StartOption"));
}

HelpEngineWrapper *HelpEngineWrapper::helpEngineWrapper = nullptr;

HelpEngineWrapper &HelpEngineWrapper::instance(const QString &collectionFile)
{
    // 该单例不能是静态的，因为要在QApplication之前删除
    if (!helpEngineWrapper)
        helpEngineWrapper = new HelpEngineWrapper(collectionFile);
    return *helpEngineWrapper;
}

void HelpEngineWrapper::removeInstance()
{
    delete helpEngineWrapper;
    helpEngineWrapper = nullptr;
}

HelpEngineWrapper::HelpEngineWrapper(const QString &collectionFile)
    : d(new HelpEngineWrapperPrivate(collectionFile))
{}

HelpEngineWrapper::~HelpEngineWrapper()
{
    const QStringList &namespaces = d->m_helpEngine->registeredDocumentations();
    for (const QString &nameSpace : namespaces) {
        const QString &docFile
            = d->m_helpEngine->documentationFileName(nameSpace);
        d->m_qchWatcher->removePath(docFile);
    }

    delete d;
}

QHelpSearchEngine *HelpEngineWrapper::searchEngine() const
{
    return d->m_helpEngine->searchEngine();
}

QHelpContentWidget *HelpEngineWrapper::contentWidget() const
{
    return d->m_helpEngine->contentWidget();
}

QUrl HelpEngineWrapper::findFile(const QUrl &url) const
{
    return d->m_helpEngine->findFile(url);
}

QByteArray HelpEngineWrapper::fileData(const QUrl &url) const
{
    return d->m_helpEngine->fileData(url);
}

const QString HelpEngineWrapper::homePage() const
{
    const QString &homePage
        = d->m_helpEngine->customValue(HomePageKey).toString();
    if (!homePage.isEmpty())
        return homePage;
    return defaultHomePage();
}

void HelpEngineWrapper::setHomePage(const QString &page)
{
    d->m_helpEngine->setCustomValue(HomePageKey, page);
}

const QString HelpEngineWrapper::defaultHomePage() const
{
    return CollectionConfiguration::defaultHomePage(*d->m_helpEngine);
}

void HelpEngineWrapper::setDefaultHomePage(const QString &page)
{
    CollectionConfiguration::setDefaultHomePage(*d->m_helpEngine, page);
}

int HelpEngineWrapper::lastTabPage() const
{
    return CollectionConfiguration::lastTabPage(*d->m_helpEngine);
}

void HelpEngineWrapper::setLastTabPage(int lastPage)
{
    CollectionConfiguration::setLastTabPage(*d->m_helpEngine, lastPage);
}

const QStringList HelpEngineWrapper::lastShownPages() const
{
    return CollectionConfiguration::lastShownPages(*d->m_helpEngine);
}

void HelpEngineWrapper::setLastShownPages(const QStringList &lastShownPages)
{
    CollectionConfiguration::setLastShownPages(*d->m_helpEngine, lastShownPages);
}

const QStringList HelpEngineWrapper::lastZoomFactors() const
{
    return CollectionConfiguration::lastZoomFactors(*d->m_helpEngine);
}

void HelpEngineWrapper::setLastZoomFactors(const QStringList &lastZoomFactors)
{
    CollectionConfiguration::setLastZoomFactors(*d->m_helpEngine, lastZoomFactors);
}

int HelpEngineWrapper::startOption() const
{
    return d->m_helpEngine->customValue(StartOptionKey, ShowLastPages).toInt();
}

void HelpEngineWrapper::setStartOption(int option)
{
    d->m_helpEngine->setCustomValue(StartOptionKey, option);
}
