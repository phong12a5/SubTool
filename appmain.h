#ifndef APPMAIN_H
#define APPMAIN_H

#include <QObject>
#include "log.h"

class AppMain : public QObject
{
    Q_OBJECT
private:
    explicit AppMain(QObject *parent = nullptr);

public:
    static AppMain* instance();

    Q_INVOKABLE bool startTest();
    Q_INVOKABLE bool stopTest();

public slots:
    void onClickedStart(bool);

private:
    static AppMain* m_instance;
};

#endif // APPMAIN_H
