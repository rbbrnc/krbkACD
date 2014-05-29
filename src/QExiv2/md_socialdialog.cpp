#include "md_socialdialog.h"
#include "ui_md_socialdialog.h"

#include "QExiv2.h"

SocialMetadataDialog::SocialMetadataDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SocialMetadataDialog),
	m_fileList(files),
	m_model(0),
    m_exiv2(0),
	m_writeComments(false)
{
	ui->setupUi(this);

	if (m_fileList.count() == 0) {
        // Empty selection!
        enableWidgets(false);
        return;
    }

    m_exiv2 = new QExiv2();

    if (m_fileList.count() == 1) {
        // Single Selection
		if (loadData(m_fileList.at(0))) {
			ui->albumTitle->setText(m_data.albumTitle());
			ui->albumDescription->setPlainText(m_data.albumDescription());
			ui->publisherName->setText(m_data.publisher());
			ui->publisherUri->setText(m_data.publisherUri());
			ui->publisherComment->setPlainText(m_data.publisherComment());
			ui->dateTimePublished->setText(m_data.datetime());
            fillCommentTable();
            // Enable comments widgets!
            m_writeComments = true;
        } else {
            enableWidgets(false);
            return;
        }
    }
//  else {// Multiple Selection;//	}
    enableWidgets(true);
}

SocialMetadataDialog::~SocialMetadataDialog()
{
	if (m_model) {
		delete m_model;
	}
    if (m_exiv2) {
        delete m_exiv2;
    }
	delete ui;
}

void SocialMetadataDialog::enableWidgets(bool enable)
{
    ui->albumTitle->setEnabled(enable);
    ui->albumDescription->setEnabled(enable);
    ui->publisherName->setEnabled(enable);
    ui->publisherUri->setEnabled(enable);

    bool enableComments = (enable && m_writeComments);
    ui->publisherComment->setEnabled(enableComments);
    ui->dateTimePublished->setEnabled(enableComments);
    ui->addCommentButton->setEnabled(enableComments);
    ui->removeCommentButton->setEnabled(enableComments);
    ui->commentsTableView->setEnabled(enableComments);
}

void SocialMetadataDialog::fillCommentTable()
{
	m_model = new QStandardItemModel(0, 4, this);

	m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Date"));
	m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Author"));
	m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Comment"));
	m_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Author Uri"));

	for (int i = 0; i < m_data.comments().count(); i++) {
		m_model->insertRow(i);
		m_model->setData(m_model->index(i, 0), m_data.comments().at(i).date);
		m_model->setData(m_model->index(i, 1), m_data.comments().at(i).author);
		m_model->setData(m_model->index(i, 2), m_data.comments().at(i).text);
		m_model->setData(m_model->index(i, 3), m_data.comments().at(i).authorUri);
	}
	ui->commentsTableView->setModel(m_model);
	//ui->commentsTableView->verticalHeader()->hide();
}

bool SocialMetadataDialog::loadData(const QString &file)
{
    if (m_exiv2->load(file)) {
        m_data = m_exiv2->socialMetadata();
        return true;
	}
    return false;
}

bool SocialMetadataDialog::saveData(const QString &file)
{
    if (m_exiv2->load(file)) {
        if (m_exiv2->setSocialMetadata(m_data, m_writeComments)) {
            return m_exiv2->save();
		}
	}
    return false;
}

void SocialMetadataDialog::on_addCommentButton_clicked()
{
	struct SocialMetadata::Comment c;
	c.date.clear();
	c.author.clear();
	c.authorUri.clear();
	c.text.clear();

	m_data.addComment(c);

	int idx = m_model->rowCount();
	m_model->insertRow(idx);
	m_model->setData(m_model->index(idx, 0), c.date);
	m_model->setData(m_model->index(idx, 1), c.author);
	m_model->setData(m_model->index(idx, 2), c.authorUri);
	m_model->setData(m_model->index(idx, 3), c.text);
}

void SocialMetadataDialog::on_removeCommentButton_clicked()
{
	QModelIndexList sel = ui->commentsTableView->selectionModel()->selectedRows();
    if (sel.isEmpty()) {
        return;
    }

    int row;
	QList<struct SocialMetadata::Comment> clist = m_data.comments();
	for (int i = 0; i < sel.count(); ++i) {
		row = sel.at(i).row();
		m_model->removeRow(row);
		clist.removeAt(row);
	}
	m_data.setComments(clist);
}

// [SLOT public]
void SocialMetadataDialog::reject()
{
	emit rejected();
	QDialog::reject();
}

void SocialMetadataDialog::accept()
{
	m_data.setAlbumTitle(ui->albumTitle->text());
	m_data.setAlbumDescription(ui->albumDescription->toPlainText());
	m_data.setPublisher(ui->publisherName->text());
	m_data.setPublisherUri(ui->publisherUri->text());
	m_data.setPublisherComment(ui->publisherComment->toPlainText());

	m_data.setDatetime(ui->dateTimePublished->text());

	QStringList sl = m_data.datetime().split(' ');
	if (!sl.isEmpty()) {
		QDate d = QDate::fromString(sl[0], "yyyy-MM-dd");
		QTime t;
		if (sl.count() > 1) {
			t = QTime::fromString(sl[1], "HH:mm:ss");
		}
		if (d.isValid() && !t.isValid()) {
			m_data.setDatetime(sl[0]);
		} else if (!d.isValid() && t.isValid()) {
			m_data.setDatetime("0000-00-00 " + sl[1]);
		} else if (!d.isValid() && !t.isValid()) {
			m_data.setDatetime(QString());
		}
	}

	if (m_writeComments) {
		QList<struct SocialMetadata::Comment> clist = m_data.comments();
		for (int i = 0; i < m_model->rowCount(); i++) {
			clist[i].date      = m_model->item(i, 0)->text();
			clist[i].author    = m_model->item(i, 1)->text();
			clist[i].text      = m_model->item(i, 2)->text();
			clist[i].authorUri = m_model->item(i, 3)->text();
		}
		m_data.setComments(clist);
	}

	for (int i = 0; i < m_fileList.count(); i++) {
		if (!saveData(m_fileList.at(i))) {
			qWarning() << "Error saving data on:" << m_fileList.at(i);
		}
	}
	QDialog::accept();
}

