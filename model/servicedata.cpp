#include "servicedata.h"
#include <QStringList>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QFileInfoList>
#include <QFileInfo>
#include <QList>
#include "log.h"

ServiceData::ServiceData(QObject *parent) : QObject(parent)
{

}

QString ServiceData::pathUploadProfile(QString url)
{
    QDir dir(url);
    QStringList finters;
    finters << "*.jpg" << "*.jpeg" << "*.jpe" << "*.jif" << "*.jfif" << "*.jfi" << "*.webp" <<
               "*.gif" << "*.png" << "*.apng" << "*.bmp" << "*.dib" << "*.tiff" << "*.tif" <<
               "*.svg" << "*.svgz" << "*.ico" << "*.xbm" ;
    dir.setNameFilters(finters);
    QFileInfoList files = dir.entryInfoList();
    QStringList lisImage;
    foreach (QFileInfo fileInfo, files) {
        QString path=fileInfo.absoluteFilePath();
        lisImage.append(path);
        QFile file(path);
        if(!file.exists()){
            LOGD << "This file is not exist";
        }else{
            LOGD << "This file was found " << file.fileName();
        }
    }
    LOGD << "files.length(): " << files.length();
    m_linkImage = lisImage.at(random(files.length() - 1));
    return m_linkImage;
}
