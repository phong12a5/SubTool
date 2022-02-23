#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <appmain.h>
#include <appmodel.h>

int main(int argc, char *argv[])
{
    QProcess::execute("Taskkill /IM chromedriver.exe /F");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("AppMain", AppMain::instance());
    engine.rootContext()->setContextProperty("AppModel", AppModel::instance());
//       engine.rootContext()->setContextProperty("applicationDirPath", QDir::currentPath());

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
