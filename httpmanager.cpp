#include "httpmanager.h"

#include <QJsonDocument>
#include <QJsonObject>

HTTPManager::HTTPManager(QObject *parent) :
    QObject(parent),
    iconDownloadManager(new QNetworkAccessManager),
    weatherAPIManager(new QNetworkAccessManager),
    frameAPIManager(new QNetworkAccessManager),
    frameDownloadManager(new QNetworkAccessManager)
{
    connect(iconDownloadManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(IconDownloadedHandler(QNetworkReply*)));

    connect(weatherAPIManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(WeatherDownloadedHandler(QNetworkReply*)));

    connect(frameDownloadManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(FrameDownloadHandler(QNetworkReply*)));

    connect(frameAPIManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(FrameDataDownloadHandler(QNetworkReply*)));
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

void HTTPManager::WeatherDownloadedHandler(QNetworkReply *reply)
{
    if (reply->error()) return;

    QString answer = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject *jsonObj = new QJsonObject(jsonResponse.object());

    emit WeatherJsonReady(jsonObj);
}

void HTTPManager::IconDownloadedHandler(QNetworkReply *reply)
{
    if (reply->error()) return;

    QPixmap *image = new QPixmap();
    image->loadFromData(reply->readAll());
    reply->deleteLater();

    emit IconReady(image);
}

void HTTPManager::sendFrameRequest()
{
    QNetworkRequest request;

    request.setUrl(QString("https://dog.ceo/api/breeds/image/random"));
    frameAPIManager->get(request);
}

void HTTPManager::sendFrameImageRequest(QString url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    frameDownloadManager->get(request);
}

void HTTPManager::FrameDataDownloadHandler(QNetworkReply *reply)
{
    if (reply->error()) {

        return;
    }

    QString answer = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject *jsonObj = new QJsonObject(jsonResponse.object());

    emit FrameJsonReady(jsonObj);
}

void HTTPManager::FrameDownloadHandler(QNetworkReply *reply)
{
    if (reply->error()) return;

    QPixmap *image = new QPixmap();
    image->loadFromData(reply->readAll());
    reply->deleteLater();

    emit FrameReady(image);
}
