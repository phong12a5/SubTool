#include <QCoreApplication>
#include <QProcess>
#include "fcareapplication.h"

int main(int argc, char *argv[])
{
//    QProcess::execute("Taskkill /IM chromedriver.exe /F");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    FCareApplication a(argc, argv);
    a.setStyle("fusion");
    a.startApplication();
    return a.exec();
}
