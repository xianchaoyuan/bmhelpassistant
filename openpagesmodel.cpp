#include "openpagesmodel.h"
#include "helpviewer.h"

OpenPagesModel::OpenPagesModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

int OpenPagesModel::rowCount(const QModelIndex &parent) const
{
    return  parent.isValid() ? 0 : m_pages.count();
}

int OpenPagesModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant OpenPagesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount() || index.column() > 0
        || role != Qt::DisplayRole)
        return QVariant();
    QString title = m_pages.at(index.row())->title();
    title.replace(QLatin1Char('&'), QLatin1String("&&"));
    return title.isEmpty() ? QLatin1String("(Untitled)") : title;
}

void OpenPagesModel::addPage(const QUrl &url, qreal zoom)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    HelpViewer *page = new HelpViewer(zoom);
    connect(page, &HelpViewer::titleChanged,
            this, &OpenPagesModel::handleTitleChanged);
    m_pages << page;
    endInsertRows();
    page->setSource(url);
}

void OpenPagesModel::removePage(int index)
{
    Q_ASSERT(index >= 0 && index < rowCount());
    beginRemoveRows(QModelIndex(), index, index);
    HelpViewer *page = m_pages.at(index);
    m_pages.removeAt(index);
    endRemoveRows();
    page->deleteLater();
}

HelpViewer *OpenPagesModel::pageAt(int index) const
{
    Q_ASSERT(index >= 0 && index < rowCount());
    return m_pages.at(index);
}

void OpenPagesModel::handleTitleChanged()
{
    HelpViewer *page = static_cast<HelpViewer *>(sender());
    const int row = m_pages.indexOf(page);
    Q_ASSERT(row != -1 );
    const QModelIndex &item = index(row, 0);
    emit dataChanged(item, item);
}
