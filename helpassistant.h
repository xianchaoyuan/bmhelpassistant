#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include "helpdocsettings.h"

class CentralWidget;
class ContentWidget;
class HelpEngineWrapper;

/**
 * @brief 主窗口
 */
class HelpAssistant : public QMainWindow
{
    Q_OBJECT

public:
    explicit HelpAssistant(const QString &collectionFile, QWidget *parent = nullptr);
    ~HelpAssistant() override;

    //! 文档的添加与删除
    bool addDocumentation(const QString &fileName);
    bool removeDocumentation(const QString &fileName);
    bool setupDocumentation(const QString &fileName);

public slots:
    //! 同步内容
    void syncContents();

private slots:
    void applyChanged();
    void showContents();
    void activateDockWidget(QWidget *w);
    void handlePageCountChanged();

private:
    void setupActions();

private:
    ContentWidget *m_contentWidget{};
    CentralWidget *m_centralWidget{};
    HelpDocSettings m_docSettings;

    HelpEngineWrapper &helpEngine;

    QAction *m_newTabAction{};
    QAction *m_closeTabAction{};
    QAction *m_pageSetupAction{};
    QAction *m_syncAction{};
};

#endif // MAINWINDOW_H
