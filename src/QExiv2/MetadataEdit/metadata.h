#ifndef METADATA_H
#define METADATA_H

#include <QStringList>
#include <QGeoLocation>

class Metadata
{
    public:
        Metadata();
        ~Metadata();

        QString m_title;   // Xmp.dc.title
        QString m_event;   // Xmp.iptcExt.Event
        QString m_desc;    // Xmp.dc.description
        QString m_comment; // image comment

        QStringList m_tags; // Xmp.dc.subject

        double m_rating;   // Xmp.xmp.Rating

        //QGeoLocation m_geoLocation;
        //LocationMetadata m_location;
        //SocialMetadata   m_social;
        //GpsMetadata      m_gps;
        //DateTimeMetadata m_dateTime;
};

#endif // METADATA_H


