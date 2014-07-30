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
        bool setLocationFormJson(const QJsonDocument &jsonDoc);

    private slots:
        void onReverseGeoCodeFinished();

    signals:
        void geoCodeFinished();
        //void reverseGeocodeFinished(const QString &data, bool error);
        void reverseGeocodeFinished();
        void reverseGeocodeError(const QString &error);

    public slots:
        void geoCode(const QGeoAddress &addr);
        void reverseGeoCode(const QGeoCoordinate &gc);

    private:
        QNetworkAccessManager *m_netManager;
        QGeoLocation m_location;
        bool m_ready;
};

#endif
