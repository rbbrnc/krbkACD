#include "metadatadialog.h"
#include "ui_metadatadialog.h"

#include <QDebug>

#include "QExiv2.h"

MetadataDialog::MetadataDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MetadataDialog),
    m_xmpDcSubjectModel(new QStringListModel(QStringList(), this)),
    m_xmpDcSubjectFilter(new QSortFilterProxyModel(this)),
	m_fileList(files)
{
	// Init resources on static libraries
	Q_INIT_RESOURCE(SpotlightWidget);

	ui->setupUi(this);

	m_xmpDcSubjectFilter->setSourceModel(m_xmpDcSubjectModel);

	if (m_fileList.count() == 0) {
		//"Empty selection!"
	} else if (m_fileList.count() == 1) {
		loadData(m_fileList.at(0));
	} else {
	}

    connect(ui->xmpDcSubjectKeyword, &SpotlightWidget::textChanged,
            m_xmpDcSubjectFilter, &QSortFilterProxyModel::setFilterFixedString);

	m_xmpDcSubjectFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
	ui->xmpDcSubjectListView->setModel(m_xmpDcSubjectFilter);

	m_updateRating = false;
}

MetadataDialog::~MetadataDialog()
{
	delete ui;
}

// Private SLOT
// Add an xmp.dc.subject keyword to the list
void MetadataDialog::on_addXmpDcSubjectButton_clicked()
{
	QString str = ui->xmpDcSubjectKeyword->text();
    if (str.isEmpty()) {
		return;
    }

	m_xmpDcSubjectModel->insertRows(m_xmpDcSubjectModel->rowCount(), 1);
    m_xmpDcSubjectModel->setData(m_xmpDcSubjectModel->index(m_xmpDcSubjectModel->rowCount() - 1), str.simplified());
	ui->xmpDcSubjectKeyword->clear();
}

// Private SLOT
// Remove an xmp.dc.subject keyword from the list
void MetadataDialog::on_removeXmpDcSubjectButton_clicked()
{
	QModelIndex idx = ui->xmpDcSubjectListView->currentIndex();
	m_xmpDcSubjectModel->removeRow(idx.row());
}

// Private SLOT
void MetadataDialog::on_xmpDcSubjectListView_doubleClicked(const QModelIndex &index)
{
	ui->xmpDcSubjectKeyword->setText(index.data().toString());
}

// Load Metadata form file.
bool MetadataDialog::loadData(const QString &file)
{
	QExiv2 *e = new QExiv2();
	if (!e->load(file)) {
		delete e;
		return false;
	}

	ui->ratingSpinBox->setValue(e->xmpTagString("Xmp.xmp.Rating", true).toDouble());

    ui->Xmp_dc_description->setPlainText(e->xmpLangAlt("Xmp.dc.description", QString(), false));
    ui->Xmp_dc_title->setText(e->xmpLangAlt("Xmp.dc.title", QString(), false));

	m_xmpDcSubjectModel->setStringList(e->xmpTagStringBag("Xmp.dc.subject", true));

    ui->Xmp_iptcExt_Event->setText(e->xmpLangAlt("Xmp.iptcExt.Event", QString(), false));

	// Image Comment
	if (e->hasComment()) {
		ui->imageComment->setPlainText(e->imgComment());
	}
	if (!e->isImgCommentWritable()) {
		ui->imageComment->setReadOnly(true);
		ui->imageCommentLabel->setText("Comment (ReadOnly)");
	}

	delete e;
	return false;
}

bool MetadataDialog::saveData(const QString &file)
{
	QExiv2 *e = new QExiv2();
	if (!e->load(file)) {
		delete e;
		return false;
	}

	e->setImgComment(ui->imageComment->toPlainText().toLatin1());

	if (ui->Xmp_iptcExt_Event->text().isEmpty()) {
		e->removeXmpTag("Xmp.iptcExt.Event");
	} else {
        e->setXmpLangAlt("Xmp.iptcExt.Event", ui->Xmp_iptcExt_Event->text());
	}

	if (ui->Xmp_dc_title->text().isEmpty()) {
		e->removeXmpTag("Xmp.dc.title");
	} else {
        e->setXmpLangAlt("Xmp.dc.title", ui->Xmp_dc_title->text());
	}

	if (ui->Xmp_dc_description->toPlainText().isEmpty()) {
		e->removeXmpTag("Xmp.dc.description");
	} else {
        e->setXmpLangAlt("Xmp.dc.description", ui->Xmp_dc_description->toPlainText());
	}

    e->setXmpBag("Xmp.dc.subject", m_xmpDcSubjectModel->stringList());

	if (m_updateRating) {
		e->setXmpTagString("Xmp.xmp.Rating", QString::number(ui->ratingSpinBox->value(), 'f', 1));
	}

	if (e->save()) {
		delete e;
		return true;
	}

    qDebug() << Q_FUNC_INFO << "Error set Xmp Data on file:" << file;
	delete e;
	return false;
}

// [SLOT public]
void MetadataDialog::reject()
{
	emit rejected();
	QDialog::reject();
}

void MetadataDialog::accept()
{
	for (int i = 0; i < m_fileList.count(); i++) {
		saveData(m_fileList.at(i));
	}
	QDialog::accept();
}

void MetadataDialog::on_ratingSpinBox_valueChanged(double)
{
	m_updateRating = true;
}
