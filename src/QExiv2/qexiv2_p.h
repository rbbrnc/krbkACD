#ifndef QEXIV2_PRIVATE_H
#define QEXIV2_PRIVATE_H

#include <QSharedData>

// The pragmas are required to be able to catch exceptions thrown by libexiv2:
// See http://gcc.gnu.org/wiki/Visibility, the section about c++ exceptions.
// They are needed for all libexiv2 versions that do not care about visibility.
#ifdef __GNUC__
#pragma GCC visibility push(default)
#endif

#include <exiv2/exiv2.hpp>

#ifdef __GNUC__
#pragma GCC visibility pop
#endif

class QExiv2DataPrivate : public QSharedData
{
	public:
		QExiv2DataPrivate();
		~QExiv2DataPrivate();

        void error(const QString &msg, Exiv2::Error &e) const;

        QString errorString(Exiv2::Error &e) const;

		bool readMetadata(const QString &filePath);
        bool readMetadata(const QByteArray &data);

        bool isMetadataWritable(Exiv2::MetadataId mid) const;

        bool setXmpTag(const char *tag);

	public:
		bool metadataValid;

		Exiv2::Image::AutoPtr image;

		std::string     imageComment;
		Exiv2::ExifData exifMetadata;
		Exiv2::IptcData iptcMetadata;
		Exiv2::XmpData  xmpMetadata;

    private:
        bool readMetadata();

};

#endif
