#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class CentralWidget;
class ContentWidget;

/**
 * @brief 主窗口
 */
class HelpAssistant : public QMainWindow
{
    Q_OBJECT

public:
    explicit HelpAssistant(const QString &collectionFile, QWidget *parent = nullptr);
    ~HelpAssistant() override;

public slots:
    //! 同步内容
    void syncContents();

private slots:
    void showContents();
    void activateDockWidget(QWidget *w);
    void handlePageCountChanged();

private:
    void setupActions();

private:
    ContentWidget *m_contentWidget{};
    CentralWidget *m_centralWidget{};

    QAction *m_newTabAction{};
    QAction *m_closeTabAction{};
    QAction *m_pageSetupAction{};
    QAction *m_syncAction{};
};

#endif // MAINWINDOW_H
