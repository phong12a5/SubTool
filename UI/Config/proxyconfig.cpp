#include "proxyconfig.h"
#include "ui_proxyconfig.h"
#include "log.h"
#include "controllermainthread.h"

ProxyConfig::ProxyConfig(QWidget *parent) :
    SubWindow(parent),
    ui(new Ui::ProxyConfig)
{
    ui->setupUi(this);
    ui->httpXproxy->setChecked(true);
    ui->threadXproxy->setMinimum(1);
    ui->httpTmProxy->setChecked(true);

    ui->checkIPBeforeRun->setChecked(SQLiteWorker::getInstance()->getCheckIpBeforeRun());
    ui->timeDelayCheckIPAfterChange->setValue(SQLiteWorker::getInstance()->getDelayIpAfterChanged());
    ui->timeChangeIP->setValue(SQLiteWorker::getInstance()->getNumberTurnToChangeIp());
    ui->changeIP->setChecked(SQLiteWorker::getInstance()->getChangeIp());
    ui->useProxy->setChecked(SQLiteWorker::getInstance()->getUseProxy());
    ui->changeIpHMA->setChecked(SQLiteWorker::getInstance()->getChangeIpHma());

    ui->normalDcom->setChecked(SQLiteWorker::getInstance()->getNommalSettingDcom());
    ui->hilinkDcom->setChecked(SQLiteWorker::getInstance()->getHilinkSettingDcom());
    ui->getNameDCOM->setText(SQLiteWorker::getInstance()->getNameDcom());
    ui->linkURLDCOM->setText(SQLiteWorker::getInstance()->getLinkUrlDcom());

    ui->useApiUserTinsoftproxy->setChecked(SQLiteWorker::getInstance()->getUseApiUser());
    ui->useApiProxyTinsoftproxy->setChecked(SQLiteWorker::getInstance()->getUseApiProxy());
    ui->apiUserTinsoftproxy->setText(SQLiteWorker::getInstance()->getApiUser());
    ui->apiProxyTinsoftproxy->setText(SQLiteWorker::getInstance()->getApiProxy());
    ui->threadIpTinproxy->setValue(SQLiteWorker::getInstance()->getNumberThreadTinSoft());

    ui->apiKeyTmproxy->setText(SQLiteWorker::getInstance()->getApiKeyTm());
    ui->threadIpTmproxy->setValue(SQLiteWorker::getInstance()->getNumberThreadTm());
    ui->httpTmProxy->setChecked(SQLiteWorker::getInstance()->getHttpTmproxy());
    ui->socketTmProxy->setChecked(SQLiteWorker::getInstance()->getSocketTmproxy());

    ui->proxyShoplike->setText(SQLiteWorker::getInstance()->getApiKeyXproxyShoplike());
    ui->threadIpProxyShoplike->setValue(SQLiteWorker::getInstance()->getNumberThreadXproxyShoplike());

    ui->apiKeyProxy6->setText(SQLiteWorker::getInstance()->getApiKeyProxyv6());
    ui->httpProxyv6->setChecked(SQLiteWorker::getInstance()->getTypeHttpProxyv6());
    ui->sock5sProxyv6->setChecked(SQLiteWorker::getInstance()->getTypeSock5sProxyv6());
    ui->proxyProxyv6->setText(SQLiteWorker::getInstance()->getProxyv6());
    ui->threadProxyv6->setValue(SQLiteWorker::getInstance()->getNumberThreadProxyv6());

    ui->httpXproxy->setChecked(SQLiteWorker::getInstance()->getHttpXroxy());
    ui->socketXproxy->setChecked(SQLiteWorker::getInstance()->getSocksXroxy());
    ui->proxyX->setText(SQLiteWorker::getInstance()->getXproxy());
    ui->threadXproxy->setValue(SQLiteWorker::getInstance()->getNumberThreadXproxy());


}

ProxyConfig::~ProxyConfig()
{
    delete ui;
}

void ProxyConfig::updateIPConfig()
{
    iPConfig.checkIpBeforeRun = (ui->checkIPBeforeRun->isChecked());
    iPConfig.delayIpAfterChanged = std::stoi(ui->timeDelayCheckIPAfterChange->text().toStdString());
    iPConfig.numberTurnToChangeIp = std::stoi(ui->timeChangeIP->text().toStdString());
    iPConfig.changeIP = ui->changeIP->isChecked();
    iPConfig.useProxy = ui->useProxy->isChecked();
    iPConfig.changeIpHMA = ui->changeIpHMA->isChecked();
}

void ProxyConfig::updateDcomSetting()
{
    dcomSetting.nommalSettingDcom = ui->normalDcom->isChecked();
    dcomSetting.hilinkSettingDcom = ui->hilinkDcom->isChecked();
    dcomSetting.nameDcom = ui->getNameDCOM->text();
    dcomSetting.linkUrl = ui->linkURLDCOM->text();
}

void ProxyConfig::updateTinSoftProxySetting()
{
    tinSoftProxySetting.useAPIUser = ui->useApiUserTinsoftproxy->isChecked();
    tinSoftProxySetting.useAPIProxy = ui->useApiProxyTinsoftproxy->isChecked();
    tinSoftProxySetting.apiUser = ui->apiUserTinsoftproxy->text();
    tinSoftProxySetting.apiProxy  = ui->apiProxyTinsoftproxy->text();
    tinSoftProxySetting.location = ui->comboBox->currentText();
    tinSoftProxySetting.numberThread = std::stoi((ui->threadIpTinproxy->text().toStdString()));
}

void ProxyConfig::updateTmProxySetting()
{
    tmProxySetting.apiKey = ui->apiKeyTmproxy->toPlainText();
    tmProxySetting.numberThread = std::stoi(ui->threadIpTmproxy->text().toStdString());
    tmProxySetting.httpProxy = ui->httpTmProxy->isChecked();
    tmProxySetting.socketProxy = ui->socketTmProxy->isChecked();
}

void ProxyConfig::updateXProxyShoplikeSetting()
{
    xProxyShoplikeSetting.apiKey = ui->proxyShoplike->toPlainText();
    xProxyShoplikeSetting.numberThread = std::stoi(ui->threadIpProxyShoplike->text().toStdString());
}

void ProxyConfig::updateProxyv6Setting()
{
    proxyv6Setting.apiKey = ui->apiKeyProxy6->text();
    proxyv6Setting.typeHttp = ui->httpProxyv6->isChecked();
    proxyv6Setting.typeSock5s = ui->sock5sProxyv6->isChecked();
    proxyv6Setting.proxy = ui->proxyProxyv6->toPlainText();
    proxyv6Setting.numberThread = std::stoi(ui->threadProxyv6->text().toStdString());
}

void ProxyConfig::updateXProxySetting()
{

    xProxy.numberThread = std::stoi(ui->threadXproxy->text().toStdString());
    xProxy.httpXproxy = ui->httpXproxy->isChecked();
    xProxy.socksXproxy = ui->socketXproxy->isChecked();
    xProxy.proxy = ui->proxyX->toPlainText();
}

IPConfig ProxyConfig::getIPConfig()
{
    updateIPConfig();
    return iPConfig;
}

DcomSetting ProxyConfig::getDcomSetting()
{
    updateDcomSetting();
    return dcomSetting;
}

TinSoftProxySetting ProxyConfig::getTinSoftProxySetting()
{
    updateTinSoftProxySetting();
    return tinSoftProxySetting;
}

TmProxySetting ProxyConfig::getTmProxySetting()
{
    updateTmProxySetting();
    return tmProxySetting;
}

XProxyShoplikeSetting ProxyConfig::getXProxyShoplikeSetting()
{
    updateXProxyShoplikeSetting();
    return xProxyShoplikeSetting;
}

Proxyv6Setting ProxyConfig::getProxyv6Setting()
{
    LOGD << "getProxyv6Setting";
    updateProxyv6Setting();
    return proxyv6Setting;
}

XProxySetting ProxyConfig::getXProxySetting()
{
    LOGD << "getXProxySetting";
    updateXProxySetting();
    return xProxy;
}


void ProxyConfig::on_pushButton_3_clicked()
{
    LOGD << "on_pushButton_3_clicked";
}

void ProxyConfig::on_checkApiUserTinsoftproxy_clicked()
{
    LOGD << "check user API tinsoft";
    CONTROLLERMAIN->checkUserKeyApiTinsoftAvailable(getTinSoftProxySetting().apiUser);
}


void ProxyConfig::on_checkApiProxyTinsoftproxy_clicked()
{
    LOGD << "check API tinsoft";
    CONTROLLERMAIN->checkProxyKeyApiTinsoftAvailable(getTinSoftProxySetting().apiProxy);
}


void ProxyConfig::on_checkApiKeyTmsoftproxy_clicked()
{
    CONTROLLERMAIN->checkApiTm(getTmProxySetting().apiKey);
}


void ProxyConfig::on_useApiUserTinsoftproxy_clicked()
{
    LOGD << "on_useApiUserTinsoftproxy_clicked";
}

void ProxyConfig::on_useApiProxyTinsoftproxy_clicked()
{
    LOGD << "on_useApiProxyTinsoftproxy_clicked";
}

void ProxyConfig::on_ipXproxy_textChanged()
{
//    QString IPs = ui->ipXproxy->toPlainText();
//    qDebug() << IPs.length();
//    if(IPs.length() %6 ==0)
//    {
//        IPs.push_front("\n");
//        ui->ipXproxy->setText(IPs);
//    }
//    QStringList listIPs = IPs.split("\n");
//    for(int i =0; i< listIPs.size(); i++)
//    {
//        //qDebug()<< "IP xproxy " << listIPs[i];
//    }
}



