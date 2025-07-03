#include "./src/frontend/mainwindow.h"
#include "./src/frontend/welcomewindow.h"

#include <QApplication>
#include <QFile>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/logo.svg"));
    //add css files
    QFile styleSheet(":styles/style.qss");
    styleSheet.open(QFile::ReadOnly);
    a.setStyleSheet(styleSheet.readAll());
    //run app
    welcomewindow w;
    w.setWindowIcon(QIcon(":/icon/logo.svg"));
    w.setWindowTitle("Jukebox_IUT - Welcome");
    w.show();
    return QApplication::exec();
}
