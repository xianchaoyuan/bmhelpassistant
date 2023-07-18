#include "helpassistant.h"
#include "helpenginewrapper.h"
#include "helpschemehandler.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

    QApplication a(argc, argv);

    HelpSchemeHandler handle;
    handle.install();

    HelpAssistant w(QString("C:/Users/yxc/Desktop/jiaoxue/teaching.qhc"));
    w.show();

    return a.exec();
}
