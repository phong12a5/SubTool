#ifndef APPMAIN_H
#define APPMAIN_H

#include <QObject>
#include "log.h"
#include <QProcess>

class AppMain : public QObject
{
    Q_OBJECT
private:
    explicit AppMain(QObject *parent = nullptr);

public:
    static AppMain* instance();

    Q_INVOKABLE bool start();
    Q_INVOKABLE bool stop();

public slots:
    void onServiceUpdated();

private:
    static AppMain* m_instance;
    QProcess* m_chromeDriverProcess = nullptr;
};

#endif // APPMAIN_H
