#include "metadata_editor_page.h"
#include "ui_metadata_editor_page.h"

#include <QMessageBox>
#include <QDebug>

MetadataEditorPage::MetadataEditorPage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MetadataEditorPage)
{
	ui->setupUi(this);

	m_manager = new QtVariantPropertyManager();

	m_exifProperties = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("EXIF"));
	m_iptcProperties = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("IPTC"));
	m_xmpProperties  = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("XMP"));
	m_commentProperties = m_manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("Image Comment"));

	m_factory = new QtVariantEditorFactory();

	ui->propertyBrowser->setFactoryForManager(m_manager, m_factory);

	ui->propertyBrowser->addProperty(m_exifProperties);
	ui->propertyBrowser->addProperty(m_iptcProperties);
	ui->propertyBrowser->addProperty(m_xmpProperties);
	ui->propertyBrowser->addProperty(m_commentProperties);

	ui->propertyBrowser->setPropertiesWithoutValueMarked(true);
	ui->propertyBrowser->setRootIsDecorated(false);

//	connect(m_manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
//		this, SLOT(propertyValueChanged(QtProperty *, const QVariant &)));

	connect(m_manager, SIGNAL(editingFinished(QtProperty *, const QVariant &)),
		this, SLOT(slotEditingFinished(QtProperty *, const QVariant &)));


//	connect(ui->propertyBrowser, SIGNAL(currentItemChanged(QtBrowserItem *)),
//		this, SLOT(slotCurrentItemChanged(QtBrowserItem *)));
}

MetadataEditorPage::~MetadataEditorPage()
{
	delete m_manager;
	delete m_factory;

	delete ui;
}

QtVariantProperty *MetadataEditorPage::testTypes(QtVariantPropertyManager *manager, struct exifData *data, bool readOnly)
{
	//qDebug() << data->key << "typeName:" << data->typeName <<  "typeId:" << "0x" + QString::number(data->typeId, 16);

	QtVariantProperty *item;

	switch (data->typeId) {
	case 1:		// unsignedByte = 1, //!< Exif BYTE type, 8-bit unsigned integer.
		item = manager->addProperty(QVariant::String, data->key);
		item->setValue(data->value);
		break;

	case 2:		// asciiString = 2, //!< Exif ASCII type, 8-bit byte.
		item = manager->addProperty(QVariant::String, data->key);
		item->setValue(data->value);
		break;

	case 3:		// unsignedShort = 3, //!< Exif SHORT type, 16-bit (2-byte) unsigned integer.
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

	if (readOnly) {
		item->setAttribute(QLatin1String("readOnly"), true);
	}

	return item;
}

// Remove previous file properties
void MetadataEditorPage::clear()
{
	QList<QtProperty *> exifSubPropertyList = m_exifProperties->subProperties();
	for (int i=0; i < exifSubPropertyList.size(); i++) {
		m_exifProperties->removeSubProperty(exifSubPropertyList.at(i));
	}

	QList<QtProperty *> iptcSubPropertyList = m_iptcProperties->subProperties();
	for (int i=0; i < iptcSubPropertyList.size(); i++) {
		m_iptcProperties->removeSubProperty(iptcSubPropertyList.at(i));
	}

	QList<QtProperty *> xmpSubPropertyList  = m_xmpProperties->subProperties();
	for (int i=0; i < xmpSubPropertyList.size(); i++) {
		m_xmpProperties->removeSubProperty(xmpSubPropertyList.at(i));
	}

	QList<QtProperty *> commentSubPropertyList  = m_commentProperties->subProperties();
	for (int i=0; i < commentSubPropertyList.size(); i++) {
		m_commentProperties->removeSubProperty(commentSubPropertyList.at(i));
	}
}

void MetadataEditorPage::setFileData(FileData fdata)
{
	m_metadata = fdata.metadata();

	clear();

	if (m_metadata.hasExif()) {
		bool ro = m_metadata.isExifWritable();
		QList<exifData> ed = m_metadata.exifDataList();
		for (int i=0; i < ed.size(); i++) {
			struct exifData data = ed.at(i);
			m_exifProperties->addSubProperty(testTypes(m_manager, &data, ro));
		}
	}

	if (m_metadata.hasIptc()) {
		bool ro = m_metadata.isIptcWritable();
		QList<exifData> ed = m_metadata.iptcDataList();
		for (int i=0; i < ed.size(); i++) {
			struct exifData data = ed.at(i);
			m_iptcProperties->addSubProperty(testTypes(m_manager, &data, ro));
		}
	}

	if (m_metadata.hasXmp()) {
		bool ro = m_metadata.isXmpWritable();
		QList<exifData> ed = m_metadata.xmpDataList();
		for (int i=0; i < ed.size(); i++) {
			struct exifData data = ed.at(i);
			m_xmpProperties->addSubProperty(testTypes(m_manager, &data, ro));
		}
	}

	// Add image comment to the view
	if (m_metadata.hasComment()) {
		QtVariantProperty *item;
		item = m_manager->addProperty(QVariant::String, QLatin1String("Comment"));
		item->setValue(m_metadata.imgComment());

		if (!m_metadata.isImgCommentWritable()) {
			item->setAttribute(QLatin1String("readOnly"), true);
		}

		m_commentProperties->addSubProperty(item);
	}
}

void MetadataEditorPage::on_cancelButton_clicked()
{

}

void MetadataEditorPage::on_updateButton_clicked()
{
	if (QMessageBox::Yes == QMessageBox::warning(this, tr("Update Metadata"),
		"Update Metadata?", QMessageBox::Yes, QMessageBox::No)) {

//		m_metadata.setImgComment("KRBK Added this Comment");
		m_metadata.save();
	}
}

// SLOT
void MetadataEditorPage::propertyValueChanged(QtProperty *prop, const QVariant &val)
{
	//qDebug() << __func__ << prop->propertyName() << "val:" << val;

	if ("Comment" == prop->propertyName()) {
		qDebug() << "MetadataEditorPage::" << __func__
			 << prop->propertyName() << "val:" << val;
	}
}

void MetadataEditorPage::slotCurrentItemChanged(QtBrowserItem *currentItem)
{
	Q_UNUSED(currentItem)
	qDebug() << __func__;
}

void MetadataEditorPage::slotEditingFinished(QtProperty *prop, const QVariant &val)
{
	qDebug() << "MetadataEditorPage::" << __func__
		 << "Prop:" << prop->propertyName()
		 << "val:" << val;

	if ("Comment" == prop->propertyName()) {
		m_metadata.setImgComment(val.toByteArray());
	}

}
