#ifndef PROXYCONFIG_H
#define PROXYCONFIG_H

#include "subwindow.h"
#include "definition.h"
#include <QCloseEvent>
#include "sqliteworker.h"
#include <QTextCharFormat>

namespace Ui {
class ProxyConfig;
}

class ProxyConfig : public SubWindow
{
    Q_OBJECT

public:
    explicit ProxyConfig(QWidget *parent = nullptr);
    ~ProxyConfig();

    IPConfig getIPConfig();
    DcomSetting getDcomSetting();
    TinSoftProxySetting getTinSoftProxySetting();
    TmProxySetting getTmProxySetting();
    XProxyShoplikeSetting getXProxyShoplikeSetting();
    Proxyv6Setting getProxyv6Setting();
    XProxySetting getXProxySetting();

private slots:

    void on_pushButton_3_clicked();

    void on_checkApiUserTinsoftproxy_clicked();

    void on_checkApiProxyTinsoftproxy_clicked();

    void on_checkApiKeyTmsoftproxy_clicked();


    void on_useApiUserTinsoftproxy_clicked();

    void on_useApiProxyTinsoftproxy_clicked();

    void on_ipXproxy_textChanged();

private:
    Ui::ProxyConfig *ui;

    IPConfig iPConfig;
    DcomSetting dcomSetting;
    TinSoftProxySetting tinSoftProxySetting;
    TmProxySetting tmProxySetting;
    XProxyShoplikeSetting xProxyShoplikeSetting;
    Proxyv6Setting proxyv6Setting;
    XProxySetting xProxy;

    void updateIPConfig();
    void updateDcomSetting();
    void updateTinSoftProxySetting();
    void updateTmProxySetting();
    void updateXProxyShoplikeSetting();
    void updateProxyv6Setting();
    void updateXProxySetting();
};

#endif // PROXYCONFIG_H
