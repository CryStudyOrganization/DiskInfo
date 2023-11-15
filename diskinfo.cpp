#include "diskinfo.h"

DiskInfo::DiskInfo()
{

}

QString DiskInfo::GetDriveDetails(const Func<QString, const QStorageInfo&>& getDriveInfo) {
    QString driveString = "";

    QList<QStorageInfo> drives = QStorageInfo::mountedVolumes();
    for (const QStorageInfo &drive : drives) {
        driveString += getDriveInfo(drive);
    }

    return driveString;
}

QString DiskInfo::GetAllLogicalDrives() {
    auto getDriveInfo = [](const QStorageInfo& drive) {
        return QString("%1, ").arg(drive.rootPath());
    };

    return "Drive: " + GetDriveDetails(getDriveInfo) + "\n";
}

QString DiskInfo::GetDriveTypeDescription(UINT driveType) {
    const char* driveTypeDescriptions[] = {
        "Unknown",  // DRIVE_UNKNOWN
        "Invalid",  // DRIVE_NO_ROOT_DIR
        "Removable",// DRIVE_REMOVABLE
        "Fixed",     // DRIVE_FIXED
        "Network",   // DRIVE_REMOTE
        "CD-ROM",    // DRIVE_CDROM
        "RAM Disk"   // DRIVE_RAMDISK
    };

    // Ensure the driveType is within a valid range
    if (driveType >= 0 && driveType < sizeof(driveTypeDescriptions) / sizeof(driveTypeDescriptions[0])) {
        return QString::fromUtf8(driveTypeDescriptions[driveType]);
    } else {
        return "Unknown";
    }
}

QString DiskInfo::GetDriveTypes() {
    auto getDriveInfo = [this](const QStorageInfo& drive) {
        QString rootPathQString = drive.rootPath();
        std::wstring rootPathWString = rootPathQString.toStdWString();
        LPCWSTR rootPath = rootPathWString.c_str();
        UINT driveType = GetDriveType(rootPath);

        if (driveType != DRIVE_NO_ROOT_DIR) {
            return QString("Drive: %1 Type: %2\n").arg(rootPathQString, GetDriveTypeDescription(driveType));
        } else {
            return QString("Drive: %1 Type: Invalid\n").arg(rootPathQString);
        }
    };

    return GetDriveDetails(getDriveInfo);
}


QString DiskInfo::GetDriveInformation() {
    auto getDriveInfo = [](const QStorageInfo& drive) {
        return QString("Drive: %1 File System: %2\n").arg(drive.rootPath(), drive.fileSystemType());
    };

    return GetDriveDetails(getDriveInfo);
}

QString DiskInfo::GetDriveSpace() {
    QString driveString = "";

    QList<QStorageInfo> drives = QStorageInfo::mountedVolumes();
    for (const QStorageInfo &drive : drives) {
        qint64 totalSpaceGB = drive.bytesTotal() / (BYTES_IN_GIGABYTE);
        qint64 freeSpaceGB = drive.bytesFree() / (BYTES_IN_GIGABYTE);

        driveString += QString("Drive: %1 Total: %2 GB Free: %3 GB\n")
                           .arg(drive.rootPath(), QString::number(totalSpaceGB), QString::number(freeSpaceGB));
    }

    return driveString;
}

QString DiskInfo::GetSystemMemory() {
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);

    if (GlobalMemoryStatusEx(&status)) {
        qint64 totalMemory = status.ullTotalPhys;
        qint64 freeMemory = status.ullAvailPhys;

        QString result = QString("Total System Memory: %1 GB\nFree System Memory: %2 GB\n")
                             .arg(totalMemory / (BYTES_IN_GIGABYTE * 1.0), 0, 'f', 2)
                             .arg(freeMemory / (BYTES_IN_GIGABYTE * 1.0), 0, 'f', 2);

        return result;
    } else {
        return "Failed to retrieve system memory information.\n";
    }
}

QString DiskInfo::GetDeviceName() {
    return QString("Computer Name: %1\n").arg(QSysInfo::machineHostName().toUtf8());
}


QString DiskInfo::GetUserNameW() {
    QString userName = qEnvironmentVariable("USERNAME");

    if (userName.isEmpty()) {
        userName = QCoreApplication::applicationFilePath().section('/', -2, -2);
    }

    return QString("Current User: %1\n").arg(userName);
}


QString DiskInfo::GetDirectoryInformation() {
    QString driveString = "";
    QDir currentDir = QDir::current();
    driveString += QString("Current Directory: %1\nTemporary Directory: %2\nHome Directory: %3\n")
                       .arg(currentDir.currentPath(), QDir::tempPath(), QDir::homePath());

    return driveString;
}

QString DiskInfo::WatchDirectoryChanges(const QString &path) {
    QString driveString = "";

    QFileSystemWatcher watcher;
    watcher.addPath(path);
    QObject::connect(&watcher, &QFileSystemWatcher::directoryChanged, [&](const QString &changedPath) {
        driveString += QString("Directory changed: %1\n").arg(changedPath);
    });

    return driveString;
}
