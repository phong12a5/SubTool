#include "fcareapplication.h"
#include "log.h"
#include "controllerworkerthread.h"
#include "controllermainthread.h"
#include "sqliteworker.h"

FCareApplication::FCareApplication(int &argc, char **argv):
    QApplication(argc, argv)
{
}

FCareApplication::~FCareApplication()
{
    LOGD;
    ControllerWorkerThread::deleteInstance();
    ControllerMainThread::deleteInstance();
}

void FCareApplication::startApplication()
{
    // init log
    LogHelper::getInstance()->initialize();

    // make instance
    SQLiteWorker::getInstance();
    CONTROLLERMAIN;
    CONTROLLERWORKER;

    // init instance
    SQLiteWorker::getInstance()->initialize();
    CONTROLLERWORKER->initialize();
    CONTROLLERMAIN->initialize();

    // start application
    CONTROLLERMAIN->startApplication();
}
