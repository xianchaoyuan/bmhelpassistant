#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "helpassistant.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Form *ui;

    HelpAssistant *m_assistant{};
};

#endif // FORM_H
