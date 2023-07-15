#ifndef HELPENGINEWRAPPERPRIVATE_H
#define HELPENGINEWRAPPERPRIVATE_H

#include <QtCore/QObject>
#include <QtCore/QFileSystemWatcher>
#include <QtHelp/QHelpEngine>

/**
 * @brief 帮助引擎包装器数据指针
 */
class HelpEngineWrapperPrivate : public QObject
{
    Q_OBJECT

    friend class HelpEngineWrapper;

private:
    explicit HelpEngineWrapperPrivate(const QString &collectionFile)
        : m_helpEngine(new QHelpEngine(collectionFile, this))
        , m_qchWatcher(new QFileSystemWatcher(this))
    {
        m_helpEngine->setUsesFilterEngine(true);
    }

    QHelpEngine * const m_helpEngine;
    QFileSystemWatcher * const m_qchWatcher;
};

#endif // HELPENGINEWRAPPERPRIVATE_H
