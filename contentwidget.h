#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include <QtCore/QUrl>
#include <QtWidgets/QWidget>

class QHelpContentWidget;

/**
 * @brief 内容窗口
 */
class ContentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContentWidget(QWidget *parent = nullptr);
    ~ContentWidget() override = default;

    //! 同步到内容
    bool syncToContent(const QUrl &url);

private slots:
    void showContextMenu(const QPoint &pos);
    void itemClicked(const QModelIndex &index);

protected:
    bool eventFilter(QObject *o, QEvent *e) override;

signals:
    void linkActivated(const QUrl &url);

private:
    QHelpContentWidget * const m_contentWidget;
};

#endif // CONTENTWIDGET_H
