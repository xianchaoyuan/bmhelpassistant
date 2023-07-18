#include "findwidget.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QToolButton>
#include <QtGui/QKeyEvent>

FindWidget::FindWidget(QWidget *parent)
    : QWidget(parent)
{
    installEventFilter(this);

    QString resourcePath = QLatin1String(":/bmhelpassistant/images/");

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(QMargins());
    hLayout->setSpacing(6);
    toolClose = setupToolButton(QString(),
                                resourcePath + QLatin1String("/closetab.png"));
    hLayout->addWidget(toolClose);
    connect(toolClose, &QAbstractButton::clicked, this, &FindWidget::close);

    editFind = new QLineEdit(this);
    hLayout->addWidget(editFind);
    editFind->setMinimumSize(QSize(150, 0));
    connect(editFind, &QLineEdit::returnPressed, this, &FindWidget::findNext);
    connect(editFind, &QLineEdit::textChanged, this, &FindWidget::textChanged);
    connect(editFind, &QLineEdit::textChanged, this, &FindWidget::updateButtons);

    toolPrevious = setupToolButton(tr("Previous"),
                                   resourcePath + QLatin1String("/backward.png"));
    connect(toolPrevious, &QAbstractButton::clicked, this, &FindWidget::findPrevious);

    hLayout->addWidget(toolPrevious);

    toolNext = setupToolButton(tr("Next"),
                               resourcePath + QLatin1String("/forward.png"));
    hLayout->addWidget(toolNext);
    connect(toolNext, &QAbstractButton::clicked, this, &FindWidget::findNext);

    checkCase = new QCheckBox(tr("Case Sensitive"), this);
    hLayout->addWidget(checkCase);

    updateButtons();
}

FindWidget::~FindWidget()
{}

void FindWidget::show()
{
    QWidget::show();
    editFind->selectAll();
    editFind->setFocus(Qt::ShortcutFocusReason);
}

void FindWidget::close()
{
    QWidget::hide();
    emit clearFind();
}

QString FindWidget::text() const
{
    return editFind->text();
}

bool FindWidget::caseSensitive() const
{
    return checkCase->isChecked();
}

bool FindWidget::eventFilter(QObject *object, QEvent *e)
{
    if (e->type() == QEvent::KeyPress) {
        if ((static_cast<QKeyEvent *>(e))->key() == Qt::Key_Escape) {
            close();
        }
    }
    return QWidget::eventFilter(object, e);
}

QToolButton *FindWidget::setupToolButton(const QString &text, const QString &icon)
{
    QToolButton *toolButton = new QToolButton(this);
    toolButton->setText(text);
    toolButton->setAutoRaise(true);
    toolButton->setIcon(QIcon(icon));
    toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    return toolButton;
}

void FindWidget::updateButtons()
{
    const bool enable = !editFind->text().isEmpty();
    toolNext->setEnabled(enable);
    toolPrevious->setEnabled(enable);
}

void FindWidget::textChanged(const QString &text)
{
    emit find(text, true);
}
