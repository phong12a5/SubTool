#ifndef UTILS_H
#define UTILS_H


#include <QString>

/**
 * @brief compareVersion, wrong format can lead to qFatal() and exit application
 * @param version1: format x.xx.xx
 * @param version2: format x.xx.xx
 * @return if version1 < version2, return < 0
 *         if version1 = version2, return 0
 *         if version1 > version2, return > 0
 */
int compareVersion(QString version1, QString version2);

/**
 * @brief checkVersionFormat xx.xx.xx
 * @param verion
 * @return
 */
bool checkVersionFormat(QString verion);

#endif // UTILS_H
