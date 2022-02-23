#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QSettings>

class AppModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool appStarted READ appStarted WRITE setAppStarted NOTIFY appStartedChanged)
    Q_PROPERTY(int maxThread READ maxThread WRITE setMaxThread NOTIFY maxThreadChanged)
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)

private:
    explicit AppModel();

public:
    static AppModel* instance();

public:
    bool appStarted();
    void setAppStarted(bool state);

    int maxThread();
    void setMaxThread(int max);

    QString token();
    void setToken(QString newToken);

signals:
    void appStartedChanged();
    void maxThreadChanged();
    void tokenChanged();

private:
    static AppModel* sInstance;

    bool m_appStarted;
    int m_maxThread;
    QString m_token;
};

#endif // APPMODEL_H
