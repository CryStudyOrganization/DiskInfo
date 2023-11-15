#ifndef DISKCHANGETRACKER_H
#define DISKCHANGETRACKER_H

#include <QCoreApplication>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QDir>


class DiskChangeTracker : public QObject
{
    Q_OBJECT

public:
    explicit DiskChangeTracker(const QString &path, QObject *parent = nullptr);

public slots:
    void startTracking();
    void stopTracking();

public slots:
    void directoryChanged(const QString &path);

private:
    QFileSystemWatcher watcher;
    QString watchedPath;
};

#endif // DISKCHANGETRACKER_H
