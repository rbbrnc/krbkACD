#include <QUrlQuery>
#include <QJsonDocument>

#include "geocoding.h"

/* Request/Response example
 *
 * request:
 *  "http://nominatim.openstreetmap.org/reverse?format=json&lat=45.4298&lon=9.29053&zoom=18&addressdetails=1"
 *
 * response:
 *
 * "{
 * 	"place_id":"3669480537",
 * 	"licence":"Data \u00a9 OpenStreetMap contributors, ODbL 1.0. http:\/\/www.openstreetmap.org\/copyright",
 * 	"osm_type":"way",
 * 	"osm_id":"76190223",
 * 	"lat":"45.427344",
 * 	"lon":"9.2924392",
 * 	"display_name":"Viale 1\u00b0 Maggio, Peschiera Borromeo, Milan, Lombardy, 20068, Italy",
 * 	"address":{
 * 		"road":"Viale 1\u00b0 Maggio",
 * 		"town":"Peschiera Borromeo",
 * 		"county":"Milan",
 *		"state":"Lombardy",
 * 		"postcode":"20068",
 * 		"country":"Italy",
 * 		"country_code":"it"
 *   }
 * }"
 */

#define URL_REVERSE_GEOCODE "http://nominatim.openstreetmap.org/reverse"

GeoCoding::GeoCoding(QObject *parent) : QObject(parent),
  m_ready(true)
{
}

QGeoLocation GeoCoding::location() const
{
    return m_location;
}

void GeoCoding::geoCode(const QGeoAddress &addr)
{
    Q_UNUSED(addr)
    //TODO
}

// REVERSE GEOCODING
void GeoCoding::reverseGeoCode(const QGeoCoordinate &gc)
{
    if (!m_ready) {
        return;
    }

    if (gc.isValid()) {
        m_location.setCoordinate(gc);
        m_location.setAddress(QGeoAddress());

        QNetworkRequest request;
        QUrlQuery query;
        QUrl url;
        url.setUrl(URL_REVERSE_GEOCODE);
        //url.addQueryItem("format", "xml");
        query.addQueryItem("format", "json");
        query.addQueryItem("lat", QString::number(gc.latitude()));
        query.addQueryItem("lon", QString::number(gc.longitude()));
        query.addQueryItem("zoom", "18");
        query.addQueryItem("addressdetails", "1");
        url.setQuery(query);
        request.setUrl(url);

        m_ready = false;
        //qDebug() << url;
        QNetworkReply *reply = m_netManager.get(request);
        connect(&m_netManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onReverseGeoCodeFinished(QNetworkReply *)));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(replyError(QNetworkReply::NetworkError)));
    }
}

bool GeoCoding::setLocationFormJson(const QByteArray &jsonData)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);
    if (error.error) {
        qWarning() << error.errorString();
        return false;
    }

    QJsonObject obj = doc.object();
    if (obj.isEmpty()) {
        qWarning() << "No JSON object found!";
        return false;
    }

    QGeoAddress addr;
    addr.setText(obj.take("display_name").toString());

    QJsonObject addrObj = obj.take("address").toObject();
    if (obj.isEmpty()) {
        qWarning() << "No JSON 'address'' object found!";
    } else {
        addr.setCity(addrObj.take("town").toString());
        addr.setCountry(addrObj.take("country").toString());
        addr.setCountryCode(addrObj.take("country_code").toString());
        addr.setCounty(addrObj.take("county").toString());
        //addr.setDistrict(addrObj.take("").toString());
        addr.setPostalCode(addrObj.take("postcode").toString());
        addr.setState(addrObj.take("state").toString());
        addr.setStreet(addrObj.take("road").toString());
    }

    m_location.setAddress(addr);
    return true;
}

void GeoCoding::onReverseGeoCodeFinished(QNetworkReply *reply)
{
    QByteArray data;

    if (reply->error()) {
        qWarning() << reply->errorString();
    } else {
        data = reply->readAll();
        if (data.isEmpty()) {
            qDebug() << __func__ << "Empty DATA";
        } else {
            //qDebug() << data;
            if (reply->error()) {
                qWarning() << reply->errorString();
            } else {
                if (setLocationFormJson(data)) {
                    emit reverseGeoCodeFinished();
                }
            }
        }
    }

    reply->deleteLater();
    reply = 0;
    m_ready = true;
}

void GeoCoding::replyError(QNetworkReply::NetworkError err)
{
    Q_UNUSED(err)
//	qDebug() << __func__ << "err=" << err;
}
