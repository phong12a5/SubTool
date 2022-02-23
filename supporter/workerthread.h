#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QMutex>

class WorkerThread: public QThread
{
    Q_OBJECT
public:
    WorkerThread();

    void runFunctionInThread(std::function<void()> runFunction);

signals:
    void finished();

private:
    void run() override;

private:
    std::function<void()> m_runRunction;
    QMutex mutex;
};

#endif // WORKERTHREAD_H
