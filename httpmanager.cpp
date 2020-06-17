#include "httpmanager.h"

#include <QJsonDocument>
#include <QJsonObject>

HTTPManager::HTTPManager(QObject *parent) :
    QObject(parent),
    iconDownloadManager(new QNetworkAccessManager),
    weatherAPIManager(new QNetworkAccessManager)
{
    connect(iconDownloadManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(IconDownloadedHandler(QNetworkReply*)));

    connect(weatherAPIManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(WeatherDownloadedHandler(QNetworkReply*)));
}

HTTPManager::~HTTPManager()
{
    delete iconDownloadManager;
    delete weatherAPIManager;
}

void HTTPManager::sendIconRequest(QString icon)
{
    QNetworkRequest request;

    QString address = "http://openweathermap.org/img/wn/" + icon + "@4x.png";
    request.setUrl(QUrl(address));
    iconDownloadManager->get(request);
}

void HTTPManager::sendWeatherRequest(QString zip)
{
    QNetworkRequest request;

    QString address = "https://api.openweathermap.org/data/2.5/weather?zip="
            + zip
            + ",us&units=imperial&appid="
            + API_KEY;

    request.setUrl(QUrl(address));
    weatherAPIManager->get(request);
}

void HTTPManager::IconDownloadedHandler(QNetworkReply *reply)
{
    if (reply->error()) return;

    QPixmap *image = new QPixmap();
    image->loadFromData(reply->readAll());
    reply->deleteLater();

    emit IconReady(image);
}

void HTTPManager::WeatherDownloadedHandler(QNetworkReply *reply)
{
    if (reply->error()) return;

    QString answer = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject *jsonObj = new QJsonObject(jsonResponse.object());

    emit WeatherJsonReady(jsonObj);
}

