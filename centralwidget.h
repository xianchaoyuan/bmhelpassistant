#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QTabBar>
#include <QtWidgets/QStackedWidget>

class HelpViewer;
class FindWidget;
class QPrinter;

/**
 * @brief 选项卡
 */
class TabBar : public QTabBar
{
    Q_OBJECT

    friend class CentralWidget;

public:
    ~TabBar();

    int addNewTab(const QString &title);
    void removeTabAt(HelpViewer *viewer);
    void setCurrent(HelpViewer *viewer);

public slots:
    void titleChanged(const QString &title);

signals:
    void currentTabChanged(HelpViewer *viewer);

private:
    explicit TabBar(QWidget *parent = nullptr);

private slots:
    void slotCurrentChanged(int index);
    void slotTabCloseRequested(int index);
    void slotCustomContextMenuRequested(const QPoint &pos);
};

/**
 * @brief 中央窗口
 */
class CentralWidget : public QWidget
{
    Q_OBJECT

    friend class OpenPagesManager;

public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget() override;

    static CentralWidget *instance();

    QUrl currentSource() const;
    QString currentTitle() const;

    HelpViewer *viewerAt(int index) const;
    HelpViewer *currentHelpViewer() const;
    int currentIndex() const;

    void connectTabBar();

    //! 前进后退状态
    bool isForwardAvailable() const;
    bool isBackwardAvailable() const;

public slots:
    void backward();
    void forward();

    void home();

    //! 打印
    void print();
    void pageSetup();

    void zoomIn();
    void zoomOut();
    void resetZoom();

    void setSource(const QUrl &url);

    void clearFind();
    void findNext();
    void findPrevious();
    void find(const QString &text, bool forward);

    void showFindWidget();

private:
    void addPage(HelpViewer *page, bool fromSearch = false);
    void removePage(int index);
    void setCurrentPage(HelpViewer *page);

    void initPrinter();

signals:
    void currentViewerChanged();
    void currentViewerLoadFinished(bool flag);

private:
    QPrinter *m_printer{};
    TabBar *m_tabBar{};
    QStackedWidget *m_stackedWidget{};
    FindWidget *m_findWidget{};
};

#endif // CENTRALWIDGET_H
