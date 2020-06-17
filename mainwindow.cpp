#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      timer(new QTimer),
      httpManager(new HTTPManager)
{
    ui->setupUi(this);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(255, 255, 255));
    ui->centralwidget->setAutoFillBackground(true);
    ui->centralwidget->setPalette(pal);

    connect(timer, SIGNAL(timeout()),
            this, SLOT(setCurrentTime()));

    setCurrentTime();
    timer->start(1000);

    connect(httpManager, SIGNAL(IconReady(QPixmap *)),
                this, SLOT(processIcon(QPixmap *)));

    connect(httpManager, SIGNAL(WeatherJsonReady(QJsonObject *)),
                this, SLOT(processWeatherJson(QJsonObject *)));

    httpManager->sendWeatherRequest(QString("98118"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCurrentTime()
{
    QTime time = QTime::currentTime();

    QTimeZone worldTimeZone = QTimeZone("America/New_York");
    QDateTime worldTime = QDateTime::currentDateTime();
    worldTime = worldTime.toTimeZone(worldTimeZone);

    ui->clock_hour->display(time.toString("hh"));
    ui->clock_min->display(time.toString("mm"));
    ui->clock_sec->display(time.toString("ss"));

    ui->clock_hour_world->display(worldTime.toString("hh"));
    ui->clock_min_world->display(worldTime.toString("mm"));
    ui->clock_sec_world->display(worldTime.toString("ss"));
}

void MainWindow::processIcon(QPixmap *image)
{
    ui->weather_icon->setPixmap(*image);
}

void MainWindow::processWeatherJson(QJsonObject *json)
{
    QString weather = json->value("weather").toArray()[0].toObject()["main"].toString();
    QString weatherDesc = json->value("weather").toArray()[0].toObject()["description"].toString();
    double temp = json->value("main").toObject()["temp"].toDouble();

    QString icon = json->value("weather").toArray()[0].toObject()["icon"].toString();
    httpManager->sendIconRequest(icon);

    ui->weather_label->setText("Current Weather: " + weather + ", temp: " + QString::number(temp));
}
