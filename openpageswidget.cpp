#include "openpageswidget.h"
#include "openpagesmodel.h"

#include <QtWidgets/QMenu>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

OpenPagesWidget::OpenPagesWidget(OpenPagesModel *model)
{
    setModel(model);
    setIndentation(0);
    setItemDelegate((m_delegate = new OpenPagesDelegate(this)));

    setTextElideMode(Qt::ElideMiddle);
    setAttribute(Qt::WA_MacShowFocusRect, false);

    viewport()->setAttribute(Qt::WA_Hover);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    header()->hide();
    header()->setStretchLastSection(false);
    header()->setSectionResizeMode(0, QHeaderView::Stretch);
    header()->setSectionResizeMode(1, QHeaderView::Fixed);
    header()->resizeSection(1, 18);

    installEventFilter(this);
    setUniformRowHeights(true);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &QAbstractItemView::clicked,
            this, &OpenPagesWidget::handleClicked);
    connect(this, &QAbstractItemView::pressed,
            this, &OpenPagesWidget::handlePressed);
    connect(this, &QWidget::customContextMenuRequested,
            this, &OpenPagesWidget::contextMenuRequested);
}

OpenPagesWidget::~OpenPagesWidget()
{}

void OpenPagesWidget::handleClicked(const QModelIndex &index)
{
    if (index.column() == 1) {
        if (model()->rowCount() > 1)
            emit closePage(index);

        QWidget *vp = viewport();
        const QPoint &cursorPos = QCursor::pos();
        QMouseEvent e(QEvent::MouseMove, vp->mapFromGlobal(cursorPos), cursorPos,
                      Qt::NoButton, {}, {});
        QCoreApplication::sendEvent(vp, &e);
    }
}

void OpenPagesWidget::handlePressed(const QModelIndex &index)
{
    if (index.column() == 0)
        emit setCurrentPage(index);

    if (index.column() == 1)
        m_delegate->pressedIndex = index;
}

void OpenPagesWidget::contextMenuRequested(QPoint pos)
{
    QModelIndex index = indexAt(pos);
    if (!index.isValid())
        return;

    if (index.column() == 1)
        index = index.sibling(index.row(), 0);
    QMenu contextMenu;
    QAction *closeEditor = contextMenu.addAction(tr("Close %1").arg(index.data()
                                                                        .toString()));
    QAction *closeOtherEditors = contextMenu.addAction(tr("Close All Except %1")
                                                           .arg(index.data().toString()));

    if (model()->rowCount() == 1) {
        closeEditor->setEnabled(false);
        closeOtherEditors->setEnabled(false);
    }

    QAction *action = contextMenu.exec(mapToGlobal(pos));
    if (action == closeEditor)
        emit closePage(index);
    else if (action == closeOtherEditors)
        emit closePagesExcept(index);
}

OpenPagesDelegate::OpenPagesDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

void OpenPagesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (option.state & QStyle::State_MouseOver) {
        if ((QApplication::mouseButtons() & Qt::LeftButton) == 0)
            pressedIndex = QModelIndex();
        QBrush brush = option.palette.alternateBase();
        if (index == pressedIndex)
            brush = option.palette.dark();
        painter->fillRect(option.rect, brush);
    }

    QStyledItemDelegate::paint(painter, option, index);

    if (index.column() == 1 && index.model()->rowCount() > 1
        && option.state & QStyle::State_MouseOver) {
        QIcon icon((option.state & QStyle::State_Selected)
                       ? ":/bmhelpassistant/images/closebutton.png"
                       : ":/bmhelpassistant/images/darkclosebutton.png");

        const QRect iconRect(option.rect.right() - option.rect.height(),
                             option.rect.top(), option.rect.height(), option.rect.height());
        icon.paint(painter, iconRect, Qt::AlignRight | Qt::AlignVCenter);
    }
}
