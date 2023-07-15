#ifndef OPENPAGESMODEL_H
#define OPENPAGESMODEL_H

#include <QtCore/QAbstractTableModel>

class HelpViewer;

/**
 * @brief 打开页模型
 */
class OpenPagesModel : public QAbstractTableModel
{
    Q_OBJECT

    friend class OpenPagesManager;

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addPage(const QUrl &url, qreal zoom = 0);
    void removePage(int index);
    HelpViewer *pageAt(int index) const;

private slots:
    void handleTitleChanged();

public:
    explicit OpenPagesModel(QObject *parent = nullptr);

private:
    QList<HelpViewer *> m_pages;
};

#endif // OPENPAGESMODEL_H
