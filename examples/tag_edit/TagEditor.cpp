#include "TagEditor.h"
#include "ui_TagEditor.h"

#include <QMessageBox>
#include <QDebug>

#include "QExiv2.h"

TagEditor::TagEditor(const QString file, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::TagEditor)
{
	ui->setupUi(this);

	m_update = false;
	m_add    = false;
	ui->saveButton->setEnabled(m_update);
	ui->addRegion->setEnabled(m_add);

	//XXX check invalid file!
	ui->fileName->setText(file);

	QPixmap thumbnail(file);
	ui->thumbnail->setPixmap(thumbnail);

	m_exiv2 = new QExiv2();
	if (m_exiv2->load(file)) {
		// Check Microsoft MP regions
		m_tagList = m_exiv2->xmpPTags();
		if (m_tagList.isEmpty()) {
			// Check MWG regions
			m_tagList = m_exiv2->xmpMWG_RegionsTags();
		}
	}

	QStringList data;
	for (int i = 0; i < m_tagList.size(); i++) {
		//m_tagList.at(i).debug();
		data.append(m_tagList.at(i).name());
	}

	m_model = new QStringListModel(this);
	m_model->setStringList(data);

	ui->regionListView->setModel(m_model);

//	connect(ui->cancelButton, SIGNAL(clicked()), qApp, SLOT(quit()));
	connect(ui->regionListView, SIGNAL(clicked(const QModelIndex &)),
		this, SLOT(updatePage(const QModelIndex &)));
}

TagEditor::~TagEditor()
{
	delete m_exiv2;
	delete m_model;
	delete ui;
}

void TagEditor::on_saveButton_clicked()
{
	if (m_update) {
		// TODO on libQExiv2
	}
}

void TagEditor::on_cancelButton_clicked()
{
	if (m_add) {
		delete m_rubberBand;
		m_add = false;
		ui->addRegion->setEnabled(m_add);
		ui->regionListView->setEnabled(true);
	}
}

void TagEditor::beginAddRegion()
{
	if (m_add) {
		return;
	}

	m_add = true;
	ui->addRegion->setEnabled(m_add);
	ui->regionListView->setEnabled(false);
}

void TagEditor::endAddRegion()
{
	if (!m_add) {
		return;
	}

	m_add = false;
	ui->addRegion->setEnabled(m_add);
	ui->regionListView->setEnabled(true);

	delete m_rubberBand;
}

void TagEditor::mousePressEvent(QMouseEvent *event)
{
	if (!m_add) {
		// Only one rubber band at time!!
		mypoint = event->pos();

		// new rectangle band
		m_rubberBand = new QRubberBand(QRubberBand::Rectangle, ui->thumbnail);
		m_rubberBand->setGeometry(QRect(mypoint, QSize()));
		m_rubberBand->show();
	}
}

void TagEditor::mouseMoveEvent(QMouseEvent *event)
{
	// Area Bounding
	m_rubberBand->setGeometry(QRect(mypoint, event->pos()).normalized());
}

void TagEditor::mouseReleaseEvent(QMouseEvent *)
{
	qDebug() << "RubberBand Geometry:" << m_rubberBand->geometry();
	//m_rubberBand->hide();

	beginAddRegion();
}

void TagEditor::updatePage(const QModelIndex &index)
{
	if (!index.isValid()) {
		ui->regionName->clear();
		ui->regionDescription->clear();
		return;
	}

	// On add/remove this may be NOT true!
	int i = index.row();

	//m_tagList.at(i).debug();
	ui->regionName->setText(m_tagList.at(i).name());
	ui->regionDescription->setPlainText(m_tagList.at(i).description());
}

void TagEditor::on_addRegion_clicked()
{
	QString str = ui->regionName->text();
	if (str.isEmpty()) {
		return;
	}

	str = str.simplified();

	m_model->insertRows(m_model->rowCount(), 1);
	if (m_model->setData(m_model->index(m_model->rowCount() - 1), str)) {
		m_update = true;
		ui->saveButton->setEnabled(m_update);
	}

        PTag *tag = new PTag();
	tag->setName(str);
	m_tagList.append(*tag);

	endAddRegion();
}

void TagEditor::on_removeRegion_clicked()
{
	QModelIndex idx = ui->regionListView->currentIndex();
	if (m_model->removeRow(idx.row())) {
		m_tagList.removeAt(idx.row());
		m_update = true;
		ui->saveButton->setEnabled(m_update);
		updatePage(ui->regionListView->currentIndex());
	}
}
