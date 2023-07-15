#ifndef OPENPAGESMANAGER_H
#define OPENPAGESMANAGER_H

#include <QtCore/QObject>

class HelpViewer;
class OpenPagesModel;

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

public slots:
    HelpViewer *createPage(const QUrl &url, bool fromSearch = false);
    HelpViewer *createBlankPage();

    void nextPage();
    void previousPage();

    void closePage(HelpViewer *page);
    void setCurrentPage(HelpViewer *page);

private slots:
    void setCurrentPage(const QModelIndex &index);
    void closePage(const QModelIndex &index);

private:
    OpenPagesManager(QObject *parent, const QUrl &cmdLineUrl);
    ~OpenPagesManager();

    //! pages 初始化
    void setupInitialPages(const QUrl &cmdLineUrl);
    void removePage(int index);

    void nextOrPreviousPage(int offset);

    OpenPagesModel *m_model{};

    static OpenPagesManager *m_instance;
};

#endif // OPENPAGESMANAGER_H
