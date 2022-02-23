#include "utils.h"
#include <QStringList>

int compareVersion(QString version1, QString version2)
{
    Q_ASSERT(checkVersionFormat(version1));
    Q_ASSERT(checkVersionFormat(version2));
    QStringList ver1List = version1.split(".", Qt::SkipEmptyParts);
    QStringList ver2List = version2.split(".", Qt::SkipEmptyParts);
    QVector<int> ver1;
    QVector<int> ver2;

    ver1 << ver1List[0].toInt() << ver1List[1].toInt() << ver1List[2].toInt();
    ver2 << ver2List[0].toInt() << ver2List[1].toInt() << ver2List[2].toInt();
    if(ver1.at(0) < ver2.at(0)){
        return -1;
    } else if (ver1.at(0) > ver2.at(0)){
        return 1;
    } else {
        if(ver1.at(1) < ver2.at(1)){
            return -1;
        } else if (ver1.at(1) > ver2.at(1)){
            return 1;
        } else {
            if(ver1.at(2) < ver2.at(2)){
                return -1;
            } else if (ver1.at(2) > ver2.at(2)){
                return 1;
            } else return 0;
        }
    }
}

bool checkVersionFormat(QString verion)
{
    QString assertMessage;
    QStringList verList = verion.split(".", Qt::SkipEmptyParts);

    if(verList.size() != 3) {
        return false;
    } else {
        bool isOk0, isOk1, isOk2;
        verList[0].toInt(&isOk0);
        verList[1].toInt(&isOk1);
        verList[2].toInt(&isOk2);
        if(!(isOk0 && isOk1 && isOk2)){
            return false;
        } else {
            return true;
        }
    }
}
