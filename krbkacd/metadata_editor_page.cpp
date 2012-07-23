#include "metadata_editor_page.h"
#include "ui_metadata_editor_page.h"

#include <QDebug>

MetadataEditorPage::MetadataEditorPage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MetadataEditorPage)
{
	ui->setupUi(this);

	variantManager = new QtVariantPropertyManager();

	exifProperties = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("EXIF"));
	iptcProperties = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("IPTC"));
	xmpProperties  = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("XMP"));
	commentProperties = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("Image Comment"));

	variantFactory = new QtVariantEditorFactory();

	ui->propertyBrowser->setFactoryForManager(variantManager, variantFactory);

	ui->propertyBrowser->addProperty(exifProperties);
	ui->propertyBrowser->addProperty(iptcProperties);
	ui->propertyBrowser->addProperty(xmpProperties);
	ui->propertyBrowser->addProperty(commentProperties);

	ui->propertyBrowser->setPropertiesWithoutValueMarked(true);
	ui->propertyBrowser->setRootIsDecorated(false);
}

MetadataEditorPage::~MetadataEditorPage()
{
	delete variantManager;
	delete variantFactory;

	delete ui;
}

QtVariantProperty *MetadataEditorPage::testTypes(QtVariantPropertyManager *manager, struct exifData *data)
{
//	qDebug() << data->key << "typeName:" << data->typeName <<  "typeId:" << data->typeId;

	QtVariantProperty *item;

	switch (data->typeId) {
	case 1:		// unsignedByte       = 1, //!< Exif BYTE type, 8-bit unsigned integer.
		item = manager->addProperty(QVariant::String, data->key);
		item->setValue(data->value);
		break;

	case 2:		// asciiString        = 2, //!< Exif ASCII type, 8-bit byte.
		item = manager->addProperty(QVariant::String, data->key);
		item->setValue(data->value);
		break;

	case 3:		// unsignedShort      = 3, //!< Exif SHORT type, 16-bit (2-byte) unsigned integer.
		item = manager->addProperty(QVariant::Int, data->key);
		item->setValue(data->value.toUShort());
		item->setAttribute(QLatin1String("minimum"), 0);
		item->setAttribute(QLatin1String("maximum"), 0xFFFF);
		item->setAttribute(QLatin1String("singleStep"), 1);
		break;

	case 4:		// unsignedLong       = 4, //!< Exif LONG type, 32-bit (4-byte) unsigned integer.
	case 5:		// unsignedRational   = 5, //!< Exif RATIONAL type, two LONGs: numerator and denumerator of a fraction.
	case 6:		// signedByte         = 6, //!< Exif SBYTE type, an 8-bit signed (twos-complement) integer.
	case 7:		// undefined          = 7, //!< Exif UNDEFINED type, an 8-bit byte that may contain anything.
	case 8:		// signedShort        = 8, //!< Exif SSHORT type, a 16-bit (2-byte) signed (twos-complement) integer.
		item = manager->addProperty(QVariant::Int, data->key);
		item->setValue(data->value.toShort());
//		item->setAttribute(QLatin1String("minimum"), 0);
//		item->setAttribute(QLatin1String("maximum"), 0xFFFF);
		item->setAttribute(QLatin1String("singleStep"), 1);
		break;

	case 9:		// signedLong         = 9, //!< Exif SLONG type, a 32-bit (4-byte) signed (twos-complement) integer.
	case 10:	// signedRational     =10, //!< Exif SRATIONAL type, two SLONGs: numerator and denumerator of a fraction.
	case 11:	// tiffFloat          =11, //!< TIFF FLOAT type, single precision (4-byte) IEEE format.
	case 12:	// tiffDouble         =12, //!< TIFF DOUBLE type, double precision (8-byte) IEEE format.
	case 13:	// tiffIfd            =13, //!< TIFF IFD type, 32-bit (4-byte) unsigned integer.
	case 0x10000:	// string        =0x10000, //!< IPTC string type.
	case 0x10001:	// date          =0x10001, //!< IPTC date type.
	case 0x10002:	// time          =0x10002, //!< IPTC time type.
	case 0x10003:	// comment       =0x10003, //!< %Exiv2 type for the Exif user comment.
	case 0x10004:	// directory     =0x10004, //!< %Exiv2 type for a CIFF directory.
	case 0x10005:	// xmpText       =0x10005, //!< XMP text type.
	case 0x10006:	// xmpAlt        =0x10006, //!< XMP alternative type.
	case 0x10007:	// xmpBag        =0x10007, //!< XMP bag type.
	case 0x10008:	// xmpSeq        =0x10008, //!< XMP sequence type.
	case 0x10009:	// langAlt       =0x10009, //!< XMP language alternative type.
	case 0x1FFFE:	// invalidTypeId =0x1fffe, //!< Invalid type id.
	case 0x1FFFF:	// lastTypeId    =0x1ffff  //!< Last type id.

	default:
		item = manager->addProperty(QVariant::String, data->key);
		item->setValue(data->value);
		break;
	}
	return item;
}

void MetadataEditorPage::setFileData(FileData fdata)
{
	m_metadata = fdata.metadata();

//	ui->propertyBrowser->clear();

	if (m_metadata.hasExif()) {
		QList<exifData> ed = m_metadata.exifDataList();
		for (int i=0; i < ed.size(); i++) {
			struct exifData data = ed.at(i);
			exifProperties->addSubProperty(testTypes(variantManager, &data));
		}
	}

	if (m_metadata.hasIptc()) {
		QList<exifData> ed = m_metadata.iptcDataList();
		for (int i=0; i < ed.size(); i++) {
			struct exifData data = ed.at(i);
			iptcProperties->addSubProperty(testTypes(variantManager, &data));
		}
	}

	if (m_metadata.hasXmp()) {
		QList<exifData> ed = m_metadata.xmpDataList();
		for (int i=0; i < ed.size(); i++) {
			struct exifData data = ed.at(i);
			xmpProperties->addSubProperty(testTypes(variantManager, &data));
		}
	}

	// Add image comment to the view
	if (m_metadata.hasComment()) {
		QtVariantProperty *item;
		item = variantManager->addProperty(QVariant::String, QLatin1String("Comment"));
		item->setValue(m_metadata.imgComment());
		item->setAttribute(QLatin1String("readOnly"), true);
		commentProperties->addSubProperty(item);
	}
}

void MetadataEditorPage::on_cancelButton_clicked()
{

}

void MetadataEditorPage::on_updateButton_clicked()
{

}
