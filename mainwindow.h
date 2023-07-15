#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class CentralWidget;
class ContentWidget;

/**
 * @brief 主窗口
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    //! 默认帮助集文件
    static QString defaultHelpCollectionFileName();

public slots:
    //! 同步内容
    void syncContents();

private slots:
    void showContents();
    void activateDockWidget(QWidget *w);

private:
    void setupActions();

private:
    ContentWidget *m_contentWidget{};
    CentralWidget *m_centralWidget{};

    QAction *m_newTabAction{};
    QAction *m_pageSetupAction{};
    QAction *m_printPreviewAction{};
    QAction *m_syncAction{};
};

#endif // MAINWINDOW_H
