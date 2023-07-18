#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QtWidgets/QWidget>

class QCheckBox;
class QLineEdit;
class QToolButton;

/**
 * @brief 查找
 */
class FindWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FindWidget(QWidget *parent = nullptr);
    ~FindWidget() override;

    void show();
    void close();

    QString text() const;
    bool caseSensitive() const;

protected:
    bool eventFilter(QObject *object, QEvent *e) override;

private:
    QToolButton *setupToolButton(const QString &text, const QString &icon);

private slots:
    void updateButtons();
    void textChanged(const QString &text);

signals:
    void clearFind();
    void findNext();
    void findPrevious();
    void find(const QString &text, bool forward);

private:
    QLineEdit *editFind;
    QToolButton *toolClose;
    QToolButton *toolPrevious;
    QToolButton *toolNext;
    QCheckBox *checkCase;
};

#endif // FINDWIDGET_H
