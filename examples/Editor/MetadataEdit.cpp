#include "MetadataEdit.h"
#include "ui_MetadataEdit.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>

#include "QExiv2.h"

MetadataEdit::MetadataEdit(const QString file, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MetadataEdit)
{
	// Init resources on static libraries
	Q_INIT_RESOURCE(SpotlightWidget);

	ui->setupUi(this);

	m_xmpUpdate = false;
	m_commentUpdate = false;
	ui->saveButton->setEnabled(m_xmpUpdate);

	//XXX check invalid file!
	ui->fileName->setText(file);
	ui->fileSize->setText(QString("%1 bytes").arg(QFileInfo(file).size()));

	QPixmap thumbnail(file);
	ui->thumbnail->setPixmap(thumbnail);
	ui->dimension->setText(QString::number(thumbnail.width()) + "x" + QString::number(thumbnail.height()));

	QStringList data;

	exiv2 = new QExiv2();
	if (exiv2->load(file)) {
		data = exiv2->xmpTagStringBag("Xmp.dc.subject", true);
		ui->ratingSpinBox->setValue(exiv2->xmpTagString("Xmp.xmp.Rating", true).toDouble());

		QString dsc = exiv2->xmpTagStringLangAlt("Xmp.dc.description", QString(), false);
		ui->description->setPlainText(dsc);

		ui->exifDateTimeOriginal->setText(exiv2->exifTagDateTime("Exif.Photo.DateTimeOriginal").toString());

		if (exiv2->hasComment()) {
			ui->imageComment->setPlainText(exiv2->imgComment());
		}
		if (!exiv2->isImgCommentWritable()) {
			ui->imageComment->setReadOnly(true);
			ui->imageCommentLabel->setText("Comment (ReadOnly)");
		}
	}

	m_model = new QStringListModel(this);
	m_model->setStringList(data);

	m_filter = new QSortFilterProxyModel(this);
	m_filter->setSourceModel(m_model);

	connect(ui->lineEdit, SIGNAL(textChanged(QString)),
		m_filter, SLOT(setFilterFixedString(QString)));

	connect(ui->description, SIGNAL(textChanged()), this, SLOT(descriptionChanged()));
	connect(ui->imageComment, SIGNAL(textChanged()), this, SLOT(imageCommentChanged()));
	connect(ui->addToolButton,    SIGNAL(clicked()), this, SLOT(addSubjectTag()));
	connect(ui->removeToolButton, SIGNAL(clicked()), this, SLOT(removeSubjectTag()));

	connect(ui->cancelButton, SIGNAL(clicked()), qApp, SLOT(quit()));

	m_filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
	ui->listView->setModel(m_filter);
}

MetadataEdit::~MetadataEdit()
{
	delete exiv2;
	delete ui;
}

void MetadataEdit::on_listView_doubleClicked(const QModelIndex &index)
{
	ui->lineEdit->setText(index.data().toString());
}

// Add a subject keyword tag to the list
void MetadataEdit::addSubjectTag()
{
	QString str = ui->lineEdit->text();
	if (str.isEmpty())
		return;

	str = str.simplified();

	m_model->insertRows(m_model->rowCount(), 1);
	if (m_model->setData(m_model->index(m_model->rowCount() - 1), str)) {
		m_xmpUpdate = true;
		ui->saveButton->setEnabled(m_xmpUpdate);
	}

	ui->lineEdit->clear();
}

// Removea subject keyword tag to the list
void MetadataEdit::removeSubjectTag()
{
	QModelIndex idx = ui->listView->currentIndex();
	if (m_model->removeRow(idx.row())) {
		m_xmpUpdate = true;
		ui->saveButton->setEnabled(m_xmpUpdate);
	}
}

void MetadataEdit::on_saveButton_clicked()
{
	if (m_xmpUpdate) {
		//m_model->sort(0);
		QString rating = QString::number(ui->ratingSpinBox->value(), 'f', 1);
		if (!exiv2->setXmpTagString("Xmp.xmp.Rating", rating)) {
			QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.xmp.Rating"), QMessageBox::Abort);
			return;
		}

                if (!exiv2->setXmpTagStringLangAlt("Xmp.dc.description", ui->description->toPlainText(), QString())) {
			QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.dc.description"), QMessageBox::Abort);
			return;
		}

		if (!exiv2->setXmpTagStringBag("Xmp.dc.subject", m_model->stringList())) {
			QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.dc.subject"), QMessageBox::Abort);
			return;
		}
	}

	if (m_commentUpdate) {
		if (!exiv2->setImgComment(ui->imageComment->toPlainText().toAscii())) {
			QMessageBox::critical(this, tr("Error"), tr("Cannot set Image Comment"), QMessageBox::Abort);
			return;
		}
	}

	if (m_commentUpdate || m_xmpUpdate) {
		exiv2->save();
	}
}

void MetadataEdit::on_ratingSpinBox_valueChanged(double)
{
	m_xmpUpdate = true;
	ui->saveButton->setEnabled(m_xmpUpdate);
}

void MetadataEdit::descriptionChanged()
{
	m_xmpUpdate = true;
	ui->saveButton->setEnabled(m_xmpUpdate);
}

void MetadataEdit::imageCommentChanged()
{
	m_commentUpdate = true;
	ui->saveButton->setEnabled(m_commentUpdate);
}
