#include "DiskChangeTracker.h"


DiskChangeTracker::DiskChangeTracker(const QString &path, QObject *parent)
    : QObject(parent), watchedPath(path)
{
}

void DiskChangeTracker::startTracking()
{
    // Set up the watcher to monitor the specified path
    watcher.addPath(watchedPath);

    // Connect the directoryChanged signal to the appropriate slot
    connect(&watcher, &QFileSystemWatcher::directoryChanged, this, &DiskChangeTracker::directoryChanged);

    qDebug() << "Start tracking changes in: " << watchedPath;
}

void DiskChangeTracker::stopTracking()
{
    // Disconnect the directoryChanged signal when tracking is stopped
    disconnect(&watcher, &QFileSystemWatcher::directoryChanged, this, &DiskChangeTracker::directoryChanged);

    // Stop watching the path
    watcher.removePath(watchedPath);

    qDebug() << "Stop tracking changes in: " << watchedPath;
}

void DiskChangeTracker::directoryChanged(const QString &path)
{
    qDebug() << "Directory changed: " << path;

    QDir dir(path);
    QStringList files = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time);

    for (const QString &file : files) {
        QString filePath = QDir::cleanPath(path + QDir::separator() + file);
        QFileInfo fileInfo(filePath);

        qDebug() << "  File/Directory: " << fileInfo.fileName();
        qDebug() << "    Path: " << fileInfo.filePath();
        qDebug() << "    Size: " << fileInfo.size() << " bytes";
        qDebug() << "    Last Modified: " << fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
        qDebug() << "    Is Directory: " << (fileInfo.isDir() ? "Yes" : "No");
        qDebug() << "    Is File: " << (fileInfo.isFile() ? "Yes" : "No");
        qDebug() << "    Is SymLink: " << (fileInfo.isSymLink() ? "Yes" : "No");
    }
}
