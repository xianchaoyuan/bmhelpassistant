#include "collectionconfiguration.h"

#include <QtHelp/QHelpEngineCore>

namespace {
    const QString DefaultHomePageKey(QLatin1String("defaultHomepage"));
    const QString LastPageKey(QLatin1String("LastTabPage"));
    const QString LastShownPagesKey(QLatin1String("LastShownPages"));
    const QString LastZoomFactorsKey(QLatin1String("LastPagesZoomTextBrowser"));
}

const QString CollectionConfiguration::DefaultZoomFactor(QLatin1String("1.0"));
const QString CollectionConfiguration::ListSeparator(QLatin1String("|"));

const QString CollectionConfiguration::defaultHomePage(const QHelpEngineCore &helpEngine)
{
    return helpEngine.customValue(DefaultHomePageKey, QLatin1String("help")).
        toString();
}

void CollectionConfiguration::setDefaultHomePage(QHelpEngineCore &helpEngine,
                                                 const QString &page)
{
    helpEngine.setCustomValue(DefaultHomePageKey, page);
}

const QStringList CollectionConfiguration::lastShownPages(const QHelpEngineCore &helpEngine)
{
    return helpEngine.customValue(LastShownPagesKey).toString().
        split(ListSeparator, Qt::SkipEmptyParts);
}

void CollectionConfiguration::setLastShownPages(QHelpEngineCore &helpEngine,
                                             const QStringList &lastShownPages)
{
    helpEngine.setCustomValue(LastShownPagesKey,
                              lastShownPages.join(ListSeparator));
}

const QStringList CollectionConfiguration::lastZoomFactors(const QHelpEngineCore &helpEngine)
{
    return helpEngine.customValue(LastZoomFactorsKey).toString().
        split(ListSeparator, Qt::SkipEmptyParts);
}

void CollectionConfiguration::setLastZoomFactors(QHelpEngineCore &helpEngine,
                                             const QStringList &lastZoomFactors)
{
    helpEngine.setCustomValue(LastZoomFactorsKey,
                              lastZoomFactors.join(ListSeparator));
}

int CollectionConfiguration::lastTabPage(const QHelpEngineCore &helpEngine)
{
    return helpEngine.customValue(LastPageKey, 1).toInt();
}

void CollectionConfiguration::setLastTabPage(QHelpEngineCore &helpEngine,
                                             int lastPage)
{
    helpEngine.setCustomValue(LastPageKey, lastPage);
}
