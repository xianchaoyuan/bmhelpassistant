#ifndef GLOBALACTIONS_H
#define GLOBALACTIONS_H

#include <QtCore/QObject>
#include <QtWidgets/QAction>

/**
 * @brief 全局actions
 */
class GlobalActions : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(GlobalActions)

public:
    static GlobalActions *instance(QObject *parent = nullptr);
    ~GlobalActions() = default;

    QList<QAction *> actionList() const { return m_actionList; }
    QAction *backwardAction() const { return m_backwardAction; }
    QAction *forwardAction() const { return m_forwardAction; }
    QAction *homeAction() const { return m_homeAction; }
    QAction *printAction() const { return m_printAction; }
    QAction *zoomInAction() const { return m_zoomInAction; }
    QAction *zoomOutAction() const { return m_zoomOutAction; }
    QAction *resetZoomAction() const { return m_resetZoomAction; }

private:
    explicit GlobalActions(QObject *parent = nullptr);

private:
    QAction *m_backwardAction;
    QAction *m_forwardAction;
    QAction *m_homeAction;
    QAction *m_printAction;
    QAction *m_zoomInAction;
    QAction *m_zoomOutAction;
    QAction *m_resetZoomAction{};

    QList<QAction *> m_actionList;
};

#endif // GLOBALACTIONS_H
