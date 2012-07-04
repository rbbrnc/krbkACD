#include <QDebug>

#include "QExiv2_p.h"

void QExiv2DataPrivate::printExiv2ExceptionError(const QString &msg, Exiv2::Error &e) const
{
	std::string s(e.what());
	qDebug() << msg.toAscii().constData()
		 << "(Error #" << e.code() << ":" << s.c_str() << ")";
}

void QExiv2DataPrivate::debug()
{
	if (exifMetadata.empty()) {
		qDebug() << "No Exif data found!";
	} else {
		// md->key()      -- Return the key of the Exifdatum. (std::string)
		// md->tag()      -- Return the tag. (uint16_t)
		// md->typeName() -- Return the name of the type. (const char *)
		// md->typeId()   -- Return the type id of the value. (TypeId)
		// md->count()    -- Return the number of components of the value. (long)
		// md->value()    -- Return the number of components of the value. (const Value &)
		//
		// enum TypeId {
		//          unsignedByte       = 1, //!< Exif BYTE type, 8-bit unsigned integer.
		//          asciiString        = 2, //!< Exif ASCII type, 8-bit byte.
		//          unsignedShort      = 3, //!< Exif SHORT type, 16-bit (2-byte) unsigned integer.
		//          unsignedLong       = 4, //!< Exif LONG type, 32-bit (4-byte) unsigned integer.
		//          unsignedRational   = 5, //!< Exif RATIONAL type, two LONGs: numerator and denumerator of a fraction.
		//          signedByte         = 6, //!< Exif SBYTE type, an 8-bit signed (twos-complement) integer.
		//          undefined          = 7, //!< Exif UNDEFINED type, an 8-bit byte that may contain anything.
		//          signedShort        = 8, //!< Exif SSHORT type, a 16-bit (2-byte) signed (twos-complement) integer.
		//          signedLong         = 9, //!< Exif SLONG type, a 32-bit (4-byte) signed (twos-complement) integer.
		//          signedRational     =10, //!< Exif SRATIONAL type, two SLONGs: numerator and denumerator of a fraction.
		//          tiffFloat          =11, //!< TIFF FLOAT type, single precision (4-byte) IEEE format.
		//          tiffDouble         =12, //!< TIFF DOUBLE type, double precision (8-byte) IEEE format.
		//          tiffIfd            =13, //!< TIFF IFD type, 32-bit (4-byte) unsigned integer.
		//          string        =0x10000, //!< IPTC string type.
		//          date          =0x10001, //!< IPTC date type.
		//          time          =0x10002, //!< IPTC time type.
		//          comment       =0x10003, //!< %Exiv2 type for the Exif user comment.
		//          directory     =0x10004, //!< %Exiv2 type for a CIFF directory.
		//          xmpText       =0x10005, //!< XMP text type.
		//          xmpAlt        =0x10006, //!< XMP alternative type.
		//          xmpBag        =0x10007, //!< XMP bag type.
		//          xmpSeq        =0x10008, //!< XMP sequence type.
		//          langAlt       =0x10009, //!< XMP language alternative type.
		//          invalidTypeId =0x1fffe, //!< Invalid type id.
		//          lastTypeId    =0x1ffff  //!< Last type id.
		// };
		//
		Exiv2::ExifData::const_iterator end = exifMetadata.end();
		for (Exiv2::ExifData::const_iterator md = exifMetadata.begin(); md != end; ++md) {
			qDebug() << QString(md->key().c_str()) << " "
				// << "0x" + QString::number(md->tag(), 16) << " "
				 << QString(md->typeName()) << " "
				 << QString::number(md->typeId()) << " "
				 << QString::number(md->count(), 10) << " "
				 << QString(md->value().toString().c_str())
				;
		}
	}
	// Print the IPTC metadata of an image
	if (iptcMetadata.empty()) {
		qDebug() << "No IPTC data found!";
	} else {
		Exiv2::IptcData::iterator end = iptcMetadata.end();
		for (Exiv2::IptcData::iterator md = iptcMetadata.begin(); md != end; ++md) {
			qDebug() << QString(md->key().c_str()) << " "
				// << "0x" + QString::number(md->tag(), 16) << " "
				 << QString(md->typeName()) << " "
				 << QString::number(md->typeId()) << " "
				 << QString::number(md->count(), 10) << " "
				 << QString(md->value().toString().c_str())
				;
		}
	}

	// -------------------------------------------------------------------------
	// Print the XMP metadata of an image
	if (xmpMetadata.empty()) {
		qDebug() <<"No XMP data found!";
	} else {
		Exiv2::XmpData::iterator end = xmpMetadata.end();
		for (Exiv2::XmpData::const_iterator md = xmpMetadata.begin(); md != end; ++md) {
			qDebug() << QString(md->key().c_str()) << " "
				// << "0x" + QString::number(md->tag(), 16) << " "
				 << QString(md->typeName()) << " "
				 << QString::number(md->typeId()) << " "
				 << QString::number(md->count(), 10) << " "
				 << QString(md->value().toString().c_str())
				;
		}
	}
}
