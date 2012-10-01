#include "MetadataEdit.h"
#include "ui_MetadataEdit.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>

#include "QExiv2.h"

MetadataEdit::MetadataEdit(QExiv2 *metadata, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MetadataEdit)
{
	// Init resources on static libraries
	Q_INIT_RESOURCE(SpotlightWidget);

	ui->setupUi(this);

	m_metadata = metadata;

	ui->ratingSpinBox->setValue(m_metadata->xmpTagString("Xmp.xmp.Rating", true).toDouble());
	ui->xmpDescription->setPlainText(m_metadata->xmpTagStringLangAlt("Xmp.dc.description", QString(), false));
	ui->xmpTitle->setText(m_metadata->xmpTagStringLangAlt("Xmp.dc.title", QString(), false));
	ui->xmpEvent->setText(m_metadata->xmpTagStringLangAlt("Xmp.iptcExt.Event", QString(), false));
//	ui->exifDateTimeOriginal->setText(m_metadata->exifTagDateTime("Exif.Photo.DateTimeOriginal").toString());

	if (m_metadata->hasComment()) {
		ui->imageComment->setPlainText(m_metadata->imgComment());
	}
	if (!m_metadata->isImgCommentWritable()) {
		ui->imageComment->setReadOnly(true);
		ui->imageCommentLabel->setText("Comment (ReadOnly)");
	}

	QStringList data;
	data = m_metadata->xmpTagStringBag("Xmp.dc.subject", true);
	m_model = new QStringListModel(this);
	m_model->setStringList(data);

	m_filter = new QSortFilterProxyModel(this);
	m_filter->setSourceModel(m_model);

	connect(ui->keyword, SIGNAL(textChanged(QString)),
		m_filter, SLOT(setFilterFixedString(QString)));

	connect(ui->xmpEvent, SIGNAL(textChanged(const QString &)), this, SLOT(xmpEventChanged(const QString &)));
	connect(ui->xmpTitle, SIGNAL(textChanged(const QString &)), this, SLOT(xmpTitleChanged(const QString &)));

	connect(ui->xmpDescription, SIGNAL(textChanged()), this, SLOT(xmpDescriptionChanged()));
	connect(ui->imageComment,   SIGNAL(textChanged()), this, SLOT(imageCommentChanged()));

	connect(ui->addToolButton,    SIGNAL(clicked()), this, SLOT(addSubjectTag()));
	connect(ui->removeToolButton, SIGNAL(clicked()), this, SLOT(removeSubjectTag()));

	m_filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
	ui->keywordsListView->setModel(m_filter);
}

MetadataEdit::~MetadataEdit()
{
	delete ui;
}

void MetadataEdit::on_listView_doubleClicked(const QModelIndex &index)
{
	ui->keyword->setText(index.data().toString());
}

// Add a subject keyword tag to the list
void MetadataEdit::addSubjectTag()
{
	QString str = ui->keyword->text();
	if (str.isEmpty())
		return;

	str = str.simplified();

	m_model->insertRows(m_model->rowCount(), 1);
	if (m_model->setData(m_model->index(m_model->rowCount() - 1), str)) {
		if (!m_metadata->setXmpTagStringBag("Xmp.dc.subject", m_model->stringList())) {
			QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.dc.subject"), QMessageBox::Abort);
		}
	}

	ui->keyword->clear();
}

// Remove a subject keyword tag to the list
void MetadataEdit::removeSubjectTag()
{
	QModelIndex idx = ui->keywordsListView->currentIndex();
	if (m_model->removeRow(idx.row())) {
		if (!m_metadata->setXmpTagStringBag("Xmp.dc.subject", m_model->stringList())) {
			QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.dc.subject"), QMessageBox::Abort);
		}
	}
}

void MetadataEdit::on_ratingSpinBox_valueChanged(double value)
{
	if (!m_metadata->setXmpTagString("Xmp.xmp.Rating", QString::number(value, 'f', 1))) {
		QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.xmp.Rating"), QMessageBox::Abort);
	}
}

void MetadataEdit::xmpEventChanged(const QString &text)
{
	if (!m_metadata->setXmpTagStringLangAlt("Xmp.iptcExt.Event", text, QString())) {
		QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.iptcExt.Event"), QMessageBox::Abort);
	}
}

void MetadataEdit::xmpTitleChanged(const QString &text)
{
	if (!m_metadata->setXmpTagStringLangAlt("Xmp.dc.title", text, QString())) {
		QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.dc.title"), QMessageBox::Abort);
	}
}

void MetadataEdit::xmpDescriptionChanged()
{
	if (!m_metadata->setXmpTagStringLangAlt("Xmp.dc.description", ui->xmpDescription->toPlainText(), QString())) {
		QMessageBox::critical(this, tr("Error"), tr("Cannot set Xmp.dc.description"), QMessageBox::Abort);
	}
}

void MetadataEdit::imageCommentChanged()
{
	if (!m_metadata->setImgComment(ui->imageComment->toPlainText().toAscii())) {
		QMessageBox::critical(this, tr("Error"), tr("Cannot set Image Comment"), QMessageBox::Abort);
	}
}
