#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QTabBar>
#include <QtWidgets/QStackedWidget>

/**
 * @brief 中央窗口
 */
class CentralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget() override = default;

    static CentralWidget *instance();

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
    void printPreview();

    void zoomIn();
    void zoomOut();
    void resetZoom();

private:
    QStackedWidget *m_stackedWidget{};
};

#endif // CENTRALWIDGET_H
