#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    watcher(this),
    watchedPath("D:/") // Replace with the path you want to monitor
{
    ui->setupUi(this);

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startTracking);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopTracking);

    DiskInfo d;
    QString res = "";

    res += "1)\n" + d.GetAllLogicalDrives() + "\n" +
           "2)\n" + d.GetDriveTypes() + "\n" +
           "3)\n" + d.GetDriveInformation() + "\n" +
           "4)\n" + d.GetDriveSpace() + "\n" +
           "5)\n" + d.GetSystemMemory() + "\n" +
           "6)\n" + d.GetDeviceName() + "\n" +
           "7)\n" + d.GetUserNameW() + "\n" +
           "8)\n" + d.GetDirectoryInformation() + "\n";

    ui->textBrowser->setPlainText(res);

    connect(&watcher, &QFileSystemWatcher::directoryChanged, this, &MainWindow::on_directoryChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startTracking()
{
    watcher.addPath(watchedPath);
    qDebug() << "Start tracking changes in: " << watchedPath;
}

void MainWindow::stopTracking()
{
    watcher.removePath(watchedPath);
    qDebug() << "Stop tracking changes in: " << watchedPath;
}


void MainWindow::on_directoryChanged(const QString &path)
{
    qDebug() << "Directory changed: " << path;

    QString changeInfo = "Directory changed: " + path + "\n";

    QDir dir(path);
    QStringList files = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Time);

    QFile logFile("log.txt");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&logFile);

        for (const QString &file : files) {
            QString filePath = QDir::cleanPath(path + QDir::separator() + file);
            QFileInfo fileInfo(filePath);

            if (fileTimestamps.contains(filePath) && fileInfo.lastModified() == fileTimestamps[filePath])
                continue;

            fileTimestamps[filePath] = fileInfo.lastModified();

            changeInfo += "  File/Directory: " + fileInfo.fileName() + "\n";
            changeInfo += "    Path: " + fileInfo.filePath() + "\n";
            changeInfo += "    Size: " + QString::number(fileInfo.size()) + " bytes\n";
            changeInfo += "    Last Modified: " + fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss") + "\n";
            changeInfo += "    Created: " + fileInfo.birthTime().toString("yyyy-MM-dd hh:mm:ss") + "\n";
            changeInfo += "    Owner: " + fileInfo.owner() + "\n";
            changeInfo += "    Permissions: " + QString::number(fileInfo.permissions(), 8) + "\n";
            changeInfo += "    Is Directory: " + QString(fileInfo.isDir() ? "Yes" : "No") + "\n";
            changeInfo += "    Is File: " + QString(fileInfo.isFile() ? "Yes" : "No") + "\n";
            changeInfo += "    Is SymLink: " + QString(fileInfo.isSymLink() ? "Yes" : "No") + "\n";

            out << changeInfo;
        }

        logFile.close();
    }

    ui->logBrowser->setPlainText(changeInfo);
}
