#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>

class HTTPManager : public QObject
{
    Q_OBJECT
public:
    explicit HTTPManager(QObject *parent = nullptr);
    ~HTTPManager();

    void sendIconRequest(QString);
    void sendWeatherRequest(QString);
    void sendFrameRequest();
    void sendFrameImageRequest(QString);

signals:
    void IconReady(QPixmap*);
    void WeatherJsonReady(QJsonObject*);
    void FrameJsonReady(QJsonObject*);
    void FrameReady(QPixmap*);

private slots:
    void IconDownloadedHandler(QNetworkReply*);
    void WeatherDownloadedHandler(QNetworkReply*);
    void FrameDataDownloadHandler(QNetworkReply*);
    void FrameDownloadHandler(QNetworkReply*);

private:
    const QString API_KEY = QString("80842829e33ecc99aa09f9e40ddbb6e0");

    QNetworkAccessManager *iconDownloadManager;
    QNetworkAccessManager *weatherAPIManager;
    QNetworkAccessManager *frameAPIManager;
    QNetworkAccessManager *frameDownloadManager;
    QByteArray downloadedData;
};

#endif // HTTPMANAGER_H
