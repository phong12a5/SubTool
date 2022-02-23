#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include <QObject>
#include "baseservice.h"
#include "controllermainthread.h"

class ServiceManager : public QObject
{
    Q_OBJECT
public:
    static ServiceManager* instance();

    QList<int> getServiceIds();
    int countService();
    void deleteService(int serviceId);
    QList<BaseService *> listService();

    // use this function is dangerous, only use when close app
    void forceDeleteAllService();

    template<typename T>
    T* getService(int serviceId){
        T* service = reinterpret_cast<T*>(serviceId);
        LOGD << "ServiceManager serviceId: " << serviceId;
        return service;
   }


    template<typename T>
    int createService() {
        if(m_listService.isEmpty()){
            CONTROLLERMAIN->setServiceRunning(true);
        }

        T* service = new T();
        LOGD << " : " << service;
        m_listService.append(service);
        return reinterpret_cast<int>(service);
    }

    void setNumberContinueClone(int num) {
        m_getContinueClone = num;
    }
    int getNumberContinueClone() {
        return m_getContinueClone;
    }

    bool checkCloneWorking(QString cloneId) {
        bool isWorking = false;
        foreach (BaseService* service, m_listService) {
            if(cloneId == service->getInfoClone()->getCloneId()) {
                isWorking = true;
                break;
            }
        }
        return isWorking;
    }


public slots:
    void onServiceFinished(int serviceId);
signals:
    void runCloneFinished(QString cloneId);
private:
    explicit ServiceManager(QObject *parent = nullptr);
    ~ServiceManager();
private:
    static ServiceManager* sInstance;

    QList<BaseService*> m_listService;
    int m_getContinueClone;

signals:
};

#endif // SERVICEMANAGER_H
