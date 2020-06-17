#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "httpmanager.h"

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QTimeZone>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setCurrentTime();
    void processIcon(QPixmap*);
    void processWeatherJson(QJsonObject*);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    HTTPManager *httpManager;
};
#endif // MAINWINDOW_H
