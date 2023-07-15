#ifndef COLLECTIONCONFIGURATION_H
#define COLLECTIONCONFIGURATION_H

#include <QtCore/QByteArray>
#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QStringList>

class QHelpEngineCore;

/**
 * @brief 帮助集配置
 */
class CollectionConfiguration
{
public:
    static const QString defaultHomePage(const QHelpEngineCore &helpEngine);
    static void setDefaultHomePage(QHelpEngineCore &helpEngine,
                                   const QString &page);

    static const QStringList lastShownPages(const QHelpEngineCore &helpEngine);
    static void setLastShownPages(QHelpEngineCore &helpEngine,
                                  const QStringList &lastShownPages);

    static const QStringList lastZoomFactors(const QHelpEngineCore &helpEngine);
    static void setLastZoomFactors(QHelpEngineCore &helPEngine,
                                   const QStringList &lastZoomFactors);

    static int lastTabPage(const QHelpEngineCore &helpEngine);
    static void setLastTabPage(QHelpEngineCore &helpEngine, int lastPage);

    static const QString DefaultZoomFactor;
    static const QString ListSeparator;
};

#endif // COLLECTIONCONFIGURATION_H
