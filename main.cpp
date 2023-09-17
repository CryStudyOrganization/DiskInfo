#include <QCoreApplication>
#include <QStorageInfo>
#include <QDir>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QHostInfo>
#include <QSysInfo>

void printDiskInfo() {
    qDebug() << "Список усіх логічних дисків в системі:";
    for (const QStorageInfo &storage : QStorageInfo::mountedVolumes()) {
        qDebug() << "Диск:" << storage.rootPath();
        qDebug() << "Тип диску:" << storage.device();
        qDebug() << "Файлова система:" << storage.fileSystemType();
        qDebug() << "Зайняте місце:" << storage.bytesTotal() - storage.bytesFree() << "байт";
        qDebug() << "Вільне місце:" << storage.bytesFree() << "байт";
        qDebug() << "-----------------------------------";
    }
}

void printSystemInfo() {
    qDebug() << "Інформація про системну пам'ять:";
    qDebug() << "Архітектура процесора:" << QSysInfo::buildCpuArchitecture();
    qDebug() << "Тип операційної системи:" << QSysInfo::productType();
    qDebug() << "-----------------------------------";

    qDebug() << "Назва комп'ютера:" << QHostInfo::localHostName();
    qDebug() << "Назва поточного користувача:" << qgetenv("USERNAME");

    qDebug() << "-----------------------------------";

    qDebug() << "Шлях до поточного системного каталогу:" << QDir::currentPath();
    qDebug() << "Шлях до тимчасового каталогу:" << QDir::tempPath();
    qDebug() << "Шлях до поточного робочого каталогу:" << QDir::homePath();
}

void onDirectoryChanged(const QString &path) {
    qDebug() << "Зміни в каталозі:" << path;
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    printDiskInfo();
    printSystemInfo();

    QFileSystemWatcher watcher;
    QString directoryToWatch = "../";
    watcher.addPath(directoryToWatch);
    QObject::connect(&watcher, &QFileSystemWatcher::directoryChanged, onDirectoryChanged);

    return a.exec();
}
