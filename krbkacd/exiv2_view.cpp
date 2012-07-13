#include "exiv2_view.h"

#include <QDebug>

Exiv2View::Exiv2View(QWidget *parent) :
	QTreeView(parent)
{
	standardModel = new QStandardItemModel;

	QStringList headerLabels;
	headerLabels << "Metadata";
	headerLabels << "Key";
	headerLabels << "Tag";
	headerLabels << "Type Name";
	headerLabels << "Count";
	headerLabels << "Value";

	standardModel->setHorizontalHeaderLabels(headerLabels);

	QStandardItem *rootNode = standardModel->invisibleRootItem();

	// Defining root items
	exifItem = new QStandardItem("EXIF");
	iptcItem = new QStandardItem("IPTC");
	xmpItem  = new QStandardItem("XMP");

	// Building up the hierarchy
	rootNode->appendRow(exifItem);
	rootNode->appendRow(iptcItem);
	rootNode->appendRow(xmpItem);

	// Register the model
	setModel(standardModel);
	//expandAll();
}

void Exiv2View::setMetadata(QStandardItem *rootItem, QString key, QString tag, QString typeName, QString count, QString value)
{
        QList<QStandardItem *> items;
        QStandardItem *item =  new QStandardItem();
        items << item;

        QStandardItem *keyItem =  new QStandardItem("Key");
        keyItem->setText(key);
        keyItem->setEditable(false);
        items << keyItem;

        QStandardItem *tagItem =  new QStandardItem("Tag");
        tagItem->setText(tag);
        tagItem->setEditable(false);
        items << tagItem;

        QStandardItem *typeNameItem =  new QStandardItem("Type Name");
        typeNameItem->setText(typeName);
        typeNameItem->setEditable(false);
        items << typeNameItem;

        QStandardItem *countItem =  new QStandardItem("Count");
        countItem->setText(count);
        countItem->setEditable(false);
        items << countItem;

        QStandardItem *valueItem =  new QStandardItem("Value");
        valueItem->setText(value);
        valueItem->setEditable(true);
        items << valueItem;

        rootItem->appendRow(items);

	// DEBUG
	//qDebug() << key << " " << tag << " " << typeName << "" << count << " " << value;
}

Exiv2View::~Exiv2View()
{
}

void Exiv2View::debug()
{
}

void Exiv2View::setFileData(FileData fdata)
{
	// Clear previous metadata
	clearMetadata();

	m_metadata = fdata.metadata();

#if 0
	QList<PTag> tl = m_metadata.xmpPTags();
	for (int i = 0; i < tl.size(); i++)
		tl.at(i).debug();
#endif
}

// Clear metadata if present!
void Exiv2View::clearMetadata()
{
	if (standardModel->rowCount() > 0) {
		qDebug() << __func__ << "rowCount=" << standardModel->rowCount();
		for (int j = 0; j < standardModel->rowCount(); j++) {
			QStandardItem *itm = standardModel->item(j, 0);
			itm->removeRows(0, itm->rowCount());
		}
	}
}

void Exiv2View::readMetadata(const QString file)
{
#if 10
	qDebug() << "________________";
	if (m_metadata.hasExif()) {
		qDebug() << "Found Exif data";
	}

	if (m_metadata.hasIptc()) {
		qDebug() << "Found Iptc data";
	}

	if (m_metadata.hasXmp()) {
		qDebug() << "Found XMP data";
	}

	if (m_metadata.hasComment()) {
		qDebug() << "Img Comment:" << m_metadata.imgComment();
	}
	qDebug() << "________________";

#else
	QByteArray ba = file.toLatin1();
	const char *c_str2 = ba.data();

	if (Exiv2::ImageType::none == Exiv2::ImageFactory::getType(c_str2)) {
		//qDebug() << "Exiv2 not recognize file";
		return;
	}

	Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(c_str2);

	image->readMetadata();

	// Print the EXIF metadata of an image
	Exiv2::ExifData &exifData = image->exifData();
	if (exifData.empty()) {
		//QString err = file + ": No Exif data found!";
		//qDebug() << err;
	} else {
		Exiv2::ExifData::const_iterator end = exifData.end();
		for (Exiv2::ExifData::const_iterator md = exifData.begin(); md != end; ++md) {
			setMetadata(exifItem, QString(md->key().c_str()),
				    "0x" + QString::number(md->tag(), 16),
				    QString(md->typeName()),
				    QString::number(md->count(), 10),
				    QString(md->value().toString().c_str()));
		}
	}

	// Print the IPTC metadata of an image
	Exiv2::IptcData &iptcData = image->iptcData();
	if (iptcData.empty()) {
		//QString err = file + ": No IPTC data found!";
		//qDebug() << err;
	} else {
		Exiv2::IptcData::iterator end = iptcData.end();
		for (Exiv2::IptcData::iterator md = iptcData.begin(); md != end; ++md) {
			setMetadata(iptcItem, QString(md->key().c_str()),
				    "0x" + QString::number(md->tag(), 16),
				    QString(md->typeName()),
				    QString::number(md->count(), 10),
				    QString(md->value().toString().c_str()));
		}
	}

	// -------------------------------------------------------------------------
	// Print the XMP metadata of an image
	Exiv2::XmpData &xmpData = image->xmpData();
	if (xmpData.empty()) {
		//QString err = file + ": No XMP data found!";
		//qDebug() << err;
	} else {
		for (Exiv2::XmpData::const_iterator md = xmpData.begin(); md != xmpData.end(); ++md) {
			setMetadata(xmpItem, QString(md->key().c_str()),
				    "0x" + QString::number(md->tag(), 16),
				    QString(md->typeName()),
				    QString::number(md->count(), 10),
				    QString(md->value().toString().c_str()));
		}
	}

#if 0
	// Get a list of preview images available in the image. The list is sorted
	// by the preview image pixel size, starting with the smallest preview.
	Exiv2::PreviewManager loader(*image);
	Exiv2::PreviewPropertiesList list = loader.getPreviewProperties();

	// Some application logic to select one of the previews from the list
	Exiv2::PreviewPropertiesList::const_iterator pos = selectPreview(list);

	// Get the selected preview image
	Exiv2::PreviewImage preview = loader.getPreviewImage(*pos);

	// Write the preview image to a file
	std::string name = "preview" + preview.extension();
	std::cout << "Writing preview" << " "
		  << preview.width() << "x" << preview.height() << " "
		  << "to file" << " " << name << std::endl;
	preview.writeFile(name);
#endif
#endif
}
