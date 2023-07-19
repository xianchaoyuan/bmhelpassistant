#ifndef OPENPAGESWIDGET_H
#define OPENPAGESWIDGET_H

#include <QtWidgets/QTreeView>
#include <QtWidgets/QStyledItemDelegate>

class OpenPagesModel;

class OpenPagesDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit OpenPagesDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    mutable QModelIndex pressedIndex;
};

/**
 * @brief 打开pages窗口
 */
class OpenPagesWidget : public QTreeView
{
    Q_OBJECT

public:
    explicit OpenPagesWidget(OpenPagesModel *model);
    ~OpenPagesWidget() override;

    void selectCurrentPage();

signals:
    void closePage(const QModelIndex &index);
    void closePagesExcept(const QModelIndex &index);
    void setCurrentPage(const QModelIndex &index);

private slots:
    void handleClicked(const QModelIndex &index);
    void handlePressed(const QModelIndex &index);
    void contextMenuRequested(QPoint pos);

private:
    OpenPagesDelegate *m_delegate;
};

#endif // OPENPAGESWIDGET_H
