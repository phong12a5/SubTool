#include "baseworker.h"

BaseWorker::BaseWorker(QObject *parent) : QObject(parent)
{
}

BaseWorker::~BaseWorker()
{
    LOGD;
}

void BaseWorker::setSerivceData(ServiceData *data)
{
    m_service_data = data;
}

void BaseWorker::setInfoClone(CommonInforClone *clone)
{
    m_infoClone = clone;
}
