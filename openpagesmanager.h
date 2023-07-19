#ifndef OPENPAGESMANAGER_H
#define OPENPAGESMANAGER_H

#include <QtCore/QObject>

class QAbstractItemView;

class HelpViewer;
class OpenPagesModel;
class OpenPagesWidget;

/**
 * @brief 打开页管理者
 */
class OpenPagesManager : public QObject
{
    Q_OBJECT

public:
    static OpenPagesManager *createInstance(QObject *parent, const QUrl &cmdLineUrl);
    static OpenPagesManager *instance();

    int pageCount() const;
    void setCurrentPage(int index);

    QAbstractItemView *openPagesWidget() const;

public slots:
    HelpViewer *createPage(const QUrl &url, bool fromSearch = false);
    HelpViewer *createBlankPage();

    void nextPage();
    void previousPage();

    void closePage(HelpViewer *page);
    void closePagesExcept(const QModelIndex &index);
    void setCurrentPage(HelpViewer *page);
    void closeCurrentPage();

private slots:
    void setCurrentPage(const QModelIndex &index);
    void closePage(const QModelIndex &index);

signals:
    void pageAdded(int index);
    void pageClosed();

private:
    OpenPagesManager(QObject *parent, const QUrl &cmdLineUrl);
    ~OpenPagesManager();

    //! pages 初始化
    void setupInitialPages(const QUrl &cmdLineUrl);
    void removePage(int index);

    void nextOrPreviousPage(int offset);

    OpenPagesModel *m_model{};
    OpenPagesWidget *m_openPagesWidget{};

    static OpenPagesManager *m_instance;
};

#endif // OPENPAGESMANAGER_H
