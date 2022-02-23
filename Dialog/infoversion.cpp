#include "infoversion.h"
#include "ui_infoversion.h"
#include <QNetworkInterface>
#include <QHostInfo>

InfoVersion::InfoVersion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoVersion)
{
    ui->setupUi(this);
    QString ver = APP_VER;
    ui->labelVersion->setText("Bạn đang sử dụng F-care phiên bản  " + ver);

    QString localhostname =  QHostInfo::localHostName();
    QString localhostIP;
    QList<QHostAddress> hostList = QHostInfo::fromName(localhostname).addresses();
    foreach (const QHostAddress& address, hostList) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address.isLoopback() == false) {
             localhostIP = address.toString();
        }
    }
    QString localMacAddress;
    QString localNetmask;
    foreach (const QNetworkInterface& networkInterface, QNetworkInterface::allInterfaces()) {
        foreach (const QNetworkAddressEntry& entry, networkInterface.addressEntries()) {
            if (entry.ip().toString() == localhostIP) {
                localMacAddress = networkInterface.hardwareAddress();
                localNetmask = entry.netmask().toString();
                break;
            }
        }
    }
    qDebug() << "Localhost name: " << localhostname;
    qDebug() << "IP = " << localhostIP;
    qDebug() << "MAC = " << localMacAddress;
    qDebug() << "Netmask = " << localNetmask;

    ui->labelMac->setText("Địa chỉ máy tính của bạn  " +localMacAddress);

}

InfoVersion::~InfoVersion()
{
    delete ui;
}
