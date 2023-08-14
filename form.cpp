#include "form.h"
#include "ui_form.h"

#include <QApplication>
#include <QDesktopWidget>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    m_assistant = new HelpAssistant(QString("C:/Users/yxc/Desktop/teaching/teaching.qhc"));
    m_assistant->show();
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    if (m_assistant) {
        m_assistant->setupDocumentation(QString("C:/Users/yxc/Desktop/teaching/wavefilter.qch"));
        m_assistant->showNormal();
        m_assistant->activateWindow();
    }
}


void Form::on_pushButton_2_clicked()
{
    if (m_assistant) {
        m_assistant->setupDocumentation(QString("C:/Users/yxc/Desktop/teaching/bmhelp.qch"));
        m_assistant->showNormal();
        m_assistant->activateWindow();
    }
}


void Form::on_pushButton_3_clicked()
{
    if (m_assistant) {
        m_assistant->setupDocumentation(QString("C:/Users/yxc/Desktop/teaching/wavefilter.qch"));
        m_assistant->showNormal();
        m_assistant->activateWindow();
    }
}

