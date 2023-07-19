#include "helpfindwidget.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QToolButton>
#include <QtGui/QKeyEvent>

HelpFindWidget::HelpFindWidget(QWidget *parent)
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
    connect(toolClose, &QAbstractButton::clicked, this, &HelpFindWidget::close);

    editFind = new QLineEdit(this);
    hLayout->addWidget(editFind);
    editFind->setMinimumSize(QSize(150, 0));
    connect(editFind, &QLineEdit::returnPressed, this, &HelpFindWidget::findNext);
    connect(editFind, &QLineEdit::textChanged, this, &HelpFindWidget::textChanged);
    connect(editFind, &QLineEdit::textChanged, this, &HelpFindWidget::updateButtons);

    toolPrevious = setupToolButton(tr("Previous"),
                                   resourcePath + QLatin1String("/backward.png"));
    connect(toolPrevious, &QAbstractButton::clicked, this, &HelpFindWidget::findPrevious);

    hLayout->addWidget(toolPrevious);

    toolNext = setupToolButton(tr("Next"),
                               resourcePath + QLatin1String("/forward.png"));
    hLayout->addWidget(toolNext);
    connect(toolNext, &QAbstractButton::clicked, this, &HelpFindWidget::findNext);

    checkCase = new QCheckBox(tr("Case Sensitive"), this);
    hLayout->addWidget(checkCase);

    updateButtons();
}

HelpFindWidget::~HelpFindWidget()
{}

void HelpFindWidget::show()
{
    QWidget::show();
    editFind->selectAll();
    editFind->setFocus(Qt::ShortcutFocusReason);
}

void HelpFindWidget::close()
{
    QWidget::hide();
    emit clearFind();
}

QString HelpFindWidget::text() const
{
    return editFind->text();
}

bool HelpFindWidget::caseSensitive() const
{
    return checkCase->isChecked();
}

bool HelpFindWidget::eventFilter(QObject *object, QEvent *e)
{
    if (e->type() == QEvent::KeyPress) {
        if ((static_cast<QKeyEvent *>(e))->key() == Qt::Key_Escape) {
            close();
        }
    }
    return QWidget::eventFilter(object, e);
}

QToolButton *HelpFindWidget::setupToolButton(const QString &text, const QString &icon)
{
    QToolButton *toolButton = new QToolButton(this);
    toolButton->setText(text);
    toolButton->setAutoRaise(true);
    toolButton->setIcon(QIcon(icon));
    toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    return toolButton;
}

void HelpFindWidget::updateButtons()
{
    const bool enable = !editFind->text().isEmpty();
    toolNext->setEnabled(enable);
    toolPrevious->setEnabled(enable);
}

void HelpFindWidget::textChanged(const QString &text)
{
    emit find(text, true);
}
