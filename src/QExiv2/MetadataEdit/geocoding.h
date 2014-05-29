#ifndef GEOCODING_H
#define GEOCODING_H

#include <QObject>
#include <QGeoLocation>
#include <QGeoAddress>
#include <QGeoCoordinate>
#include <QtNetwork>

class GeoCoding : public QObject
{
    Q_OBJECT

    public:
        explicit GeoCoding(QObject *parent = 0);

        QGeoLocation location() const;

    private:
        bool setLocationFormJson(const QByteArray &jsonData);

    private slots:
        void onReverseGeoCodeFinished(QNetworkReply *reply);
        void replyError(QNetworkReply::NetworkError err);

    signals:
        void geoCodeFinished();
        void reverseGeoCodeFinished();

    public slots:
        void geoCode(const QGeoAddress &addr);
        void reverseGeoCode(const QGeoCoordinate &gc);

    private:
        QNetworkAccessManager m_netManager;
        QGeoLocation m_location;
        bool m_ready;
};

#endif
