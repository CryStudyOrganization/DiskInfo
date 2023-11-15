#ifndef DISKINFO_H
#define DISKINFO_H

#include <QDir>
#include <QString>
#include <QSysInfo>
#include <QStorageInfo>
#include <QFileSystemWatcher>
#include <QCoreApplication>
#include <QSettings>

#include <functional>
#include <iostream>
#include <windows.h>

class DiskInfo
{
public:
    template <typename TResult, typename... Args>
    using Func = std::function<TResult(Args...)>;

    const qint64 BYTES_IN_GIGABYTE = 1024 * 1024 * 1024;

    DiskInfo();

    QString GetAllLogicalDrives();                          //1
    QString GetDriveTypes();                                //2
    QString GetDriveInformation();                          //3
    QString GetDriveSpace();                                //4
    QString GetSystemMemory();                              //5
    QString GetDeviceName();                                //6
    QString GetUserNameW();                                  //7
    QString GetDirectoryInformation();                      //8
    QString WatchDirectoryChanges(const QString &path);     //9

private:
    QString GetDriveTypeDescription(UINT driveType);
    QString GetDriveDetails(const Func<QString, const QStorageInfo&>& getDriveInfo);

};

#endif // DISKINFO_H
