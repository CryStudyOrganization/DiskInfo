#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QString>
#include <QPushButton>

#include "diskinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void startTracking();
    void stopTracking();

private:
    Ui::MainWindow *ui;
    QPushButton *startButton; // Updated: Use pointers for buttons
    QPushButton *stopButton;  // Updated: Use pointers for buttons

    QFileSystemWatcher watcher;
    QString watchedPath;

    QMap<QString, QDateTime> fileTimestamps;

private slots:
    void on_directoryChanged(const QString &path);
};

#endif // MAINWINDOW_H
