#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class CentralWidget;

/**
 * @brief 主窗口
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    //! 同步内容
    void syncContents();

private slots:
    void showContents();
    void showOpenPages();

private:
    void setupActions();

private:
    CentralWidget *m_centralWidget{};

    QAction *m_newTabAction{};
    QAction *m_pageSetupAction{};
    QAction *m_printPreviewAction{};
    QAction *m_syncAction{};
};

#endif // MAINWINDOW_H
