#include "infowidget.h"
#include "ui_infowidget.h"

#include <QFile>
#include <QFileInfo>
#include <QImageReader>

#include "QMagic.h"
#include "QExiv2.h"

InfoWidget::InfoWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::InfoWidget)
{
	ui->setupUi(this);
}

InfoWidget::~InfoWidget()
{
	delete ui;
}

void InfoWidget::setFile(const QString &file)
{
	QString mimeType = QMagic::mimeType(file);
	bool isImage = mimeType.contains("image");

	QFileInfo fi(file);

	ui->pathLabel->setText(fi.canonicalPath());
	ui->fileLabel->setText(fi.fileName());

	//ui->kindLabel->setText(QMagic::mimeDescription(file));
	ui->kindLabel->setText(QMagic::mimeType(file));
	ui->sizeLabel->setText(QString("%1 bytes").arg(fi.size()));
	ui->modifiedLabel->setText(fi.lastModified().toString());

	ui->createdLabel->setText(fi.created().toString());
	ui->ownerLabel->setText(fi.owner());
	ui->groupLabel->setText(fi.group());

	QFile::Permissions fp = fi.permissions();
	QString perm;
	perm  = (fp & QFile::ReadOwner)  ? 'r' : '-';
	perm += (fp & QFile::WriteOwner) ? 'w' : '-';
	perm += (fp & QFile::ExeOwner)   ? "x " : "- ";
	perm += (fp & QFile::ReadGroup)  ? 'r' : '-';
	perm += (fp & QFile::WriteGroup) ? 'w' : '-';
	perm += (fp & QFile::ExeGroup)   ? "x " : "- ";
	perm += (fp & QFile::ReadOther)  ? 'r' : '-';
	perm += (fp & QFile::WriteOther) ? 'w' : '-';
	perm += (fp & QFile::ExeOther)   ? "x " : "- ";
	ui->permissionsLabel->setText(perm);

	if (isImage) {
		// Using QImageReader for get image dimension.
		QImageReader ir(file);
		ui->dimensionLabel->setText(QString("%1 x %2").arg(ir.size().width()).arg(ir.size().height()));
	} else {
		ui->dimensionLabel->setText("-");
	}

	// Update metadata labels
	bool hasExifData = false;
	bool hasIptcData = false;
	bool hasXmpData = false;
	bool hasCommentData = false;
	bool isVideo = mimeType.contains("video");

	if (isImage || isVideo) {
		QExiv2 metadata;
		metadata.load(file);
		if (metadata.isValid()) {
			hasExifData = metadata.hasExif();
			hasIptcData = metadata.hasIptc();
			hasXmpData  = metadata.hasXmp();
			hasCommentData = metadata.hasComment();
		}
	}

	ui->exifLabel->setText((hasExifData) ? "Yes" : "-");
	ui->iptcLabel->setText((hasIptcData) ? "Yes" : "-");
	ui->xmpLabel->setText((hasXmpData) ? "Yes" : "-");
	ui->commentLabel->setText((hasCommentData) ? "Yes" : "-");
}


