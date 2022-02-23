#include "workerthread.h"

WorkerThread::WorkerThread()
{

}

void WorkerThread::runFunctionInThread(std::function<void ()> runFunction)
{
    mutex.lock();
    m_runRunction = runFunction;
    this->start();
    mutex.unlock();
}

void WorkerThread::run()
{
    mutex.lock();
    m_runRunction();
    emit finished();
    mutex.unlock();
}
