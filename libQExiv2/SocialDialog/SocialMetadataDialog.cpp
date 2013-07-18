#include "SocialMetadataDialog.h"
#include "ui_SocialMetadataDialog.h"

#include "QExiv2.h"

SocialMetadataDialog::SocialMetadataDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SocialMetadataDialog),
	m_fileList(files),
	m_model(0),
	m_writeComments(false)
{
	ui->setupUi(this);
	//ui->commentsTableView->setShowGrid(false);

	if (m_fileList.count() == 0) {
		//"Empty selection!"
	} else if (m_fileList.count() == 1) {
		if (loadData(m_fileList.at(0))) {
			ui->albumTitle->setText(m_data.albumTitle);
			ui->albumDescription->setPlainText(m_data.albumDescription);
			ui->publisherName->setText(m_data.publisherName);
			ui->publisherUri->setText(m_data.publisherUri);
			ui->publisherComment->setPlainText(m_data.publisherComment);
			ui->dateTimePublished->setText(m_data.datetimePublished);
			fillCommentTable();
			m_writeComments = true;
		}
	} else {
		// Multiple Selection;
	}
}

SocialMetadataDialog::~SocialMetadataDialog()
{
	if (m_model) {
		delete m_model;
	}
	delete ui;
}

void SocialMetadataDialog::fillCommentTable()
{
	m_model = new QStandardItemModel(0, 4, this);

	m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Date"));
	m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Author"));
	m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Comment"));
	m_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Author Uri"));

	for (int i = 0; i < m_data.comments.count(); i++) {
		m_model->insertRow(i);
		m_model->setData(m_model->index(i, 0), m_data.comments.at(i).date);
		m_model->setData(m_model->index(i, 1), m_data.comments.at(i).author);
		m_model->setData(m_model->index(i, 2), m_data.comments.at(i).comment);
		m_model->setData(m_model->index(i, 3), m_data.comments.at(i).authorUri);
	}
	ui->commentsTableView->setModel(m_model);
	//ui->commentsTableView->verticalHeader()->hide();
}

bool SocialMetadataDialog::loadData(const QString &file)
{
	QExiv2 *e = new QExiv2();
	if (!e->load(file)) {
		delete e;
		return false;
	}

	if (!e->xmpRegisterNamespace("social/", "social")) {
		return false;
	}

	m_data.albumTitle       = e->xmpTagString("Xmp.social.AlbumTitle", true);
	m_data.albumDescription = e->xmpTagString("Xmp.social.AlbumDescription", true);
	m_data.publisherName    = e->xmpTagString("Xmp.social.PublisherName", true);
	m_data.publisherUri     = e->xmpTagString("Xmp.social.PublisherUri", true);
	m_data.datetimePublished = e->xmpTagString("Xmp.social.DateTimePublished", true);

	// Xmp.social.PublisherComment can be mapped on dc:description
	m_data.publisherComment = e->xmpTagString("Xmp.social.PublisherComment");

	for (int i = 1; ; i++) {
		if (e->xmpTagString(QString("Xmp.social.Comments[%1]").arg(i)).isEmpty()) {
			break;
		}
		struct SocialMetadata::comment cmn;
		cmn.date = e->xmpTagString(QString("Xmp.social.Comments[%1]/social:CommentDate").arg(i));
		cmn.author = e->xmpTagString(QString("Xmp.social.Comments[%1]/social:CommentAuthor").arg(i));
		cmn.authorUri = e->xmpTagString(QString("Xmp.social.Comments[%1]/social:CommentAuthorUri").arg(i));
		cmn.comment = e->xmpTagString(QString("Xmp.social.Comments[%1]/social:CommentText").arg(i));
		m_data.comments.append(cmn);
	}

	delete e;

	return true;
}

bool SocialMetadataDialog::saveData(const QString &file)
{
	QExiv2 *e = new QExiv2();
	if (!e->load(file)) {
		delete e;
		return false;
	}
	e->setXmpTagString("Xmp.social.AlbumTitle", m_data.albumTitle);
	e->setXmpTagString("Xmp.social.AlbumDescription", m_data.albumDescription);
	e->setXmpTagString("Xmp.social.PublisherName", m_data.publisherName);
	e->setXmpTagString("Xmp.social.PublisherUri", m_data.publisherUri);
	e->setXmpTagString("Xmp.social.PublisherComment", m_data.publisherComment);
	e->setXmpTagString("Xmp.social.DateTimePublished", m_data.datetimePublished);

	if (m_writeComments) {
		e->removeXmpBag("Xmp.social.Comments", 19);
		if (m_data.comments.count() > 0) {
			e->setXmpTagBag("Xmp.social.Comments");
			for (int i = 0; i < m_data.comments.count(); i++) {
				e->setXmpTagString(QString("Xmp.social.Comments[%1]/social:CommentDate").arg(i+1), m_data.comments.at(i).date);
				e->setXmpTagString(QString("Xmp.social.Comments[%1]/social:CommentAuthor").arg(i+1),  m_data.comments.at(i).author);
				e->setXmpTagString(QString("Xmp.social.Comments[%1]/social:CommentAuthorUri").arg(i+1), m_data.comments.at(i).authorUri);
				e->setXmpTagString(QString("Xmp.social.Comments[%1]/social:CommentText").arg(i+1), m_data.comments.at(i).comment);
			}
		}
	}

	if (e->save()) {
		delete e;
		return true;
	}

	delete e;
	return false;
}

void SocialMetadataDialog::on_addCommentButton_clicked()
{
	struct SocialMetadata::comment c;
	c.date = "";
	c.author = "";
	c.authorUri = "";
	c.comment = "";
	m_data.comments.append(c);

	int idx = m_model->rowCount();
	m_model->insertRow(idx);
	m_model->setData(m_model->index(idx, 0), c.date);
	m_model->setData(m_model->index(idx, 1), c.author);
	m_model->setData(m_model->index(idx, 2), c.authorUri);
	m_model->setData(m_model->index(idx, 3), c.comment);
}

void SocialMetadataDialog::on_removeCommentButton_clicked()
{
	QModelIndexList sel = ui->commentsTableView->selectionModel()->selectedRows();
	for (int i = 0; i < sel.count(); ++i) {
		int row = sel.at(i).row();
		m_model->removeRow(row);
		m_data.comments.removeAt(row);
	}
}

// [SLOT public]
void SocialMetadataDialog::reject()
{
	emit rejected();
	QDialog::reject();
}

void SocialMetadataDialog::accept()
{
	m_data.albumTitle       = ui->albumTitle->text();
	m_data.albumDescription = ui->albumDescription->toPlainText();
	m_data.publisherName    = ui->publisherName->text();
	m_data.publisherUri     = ui->publisherUri->text();
	m_data.publisherComment = ui->publisherComment->toPlainText();

	m_data.datetimePublished = ui->dateTimePublished->text();
	QStringList sl = m_data.datetimePublished.split(' ');
	if (!sl.isEmpty()) {
		QDate d = QDate::fromString(sl[0], "yyyy-MM-dd");
		QTime t;
		if (sl.count() > 1) {
			t = QTime::fromString(sl[1], "HH:mm:ss");
		}
		if (d.isValid() && !t.isValid()) {
			m_data.datetimePublished = sl[0];
		} else if (!d.isValid() && t.isValid()) {
			m_data.datetimePublished = "0000-00-00 " + sl[1];
		} else if (!d.isValid() && !t.isValid()) {
			m_data.datetimePublished.clear();
		}
	}

	if (m_writeComments) {
		for (int i = 0; i < m_model->rowCount(); i++) {
			m_data.comments[i].date      = m_model->item(i, 0)->text();
			m_data.comments[i].author    = m_model->item(i, 1)->text();
			m_data.comments[i].comment   = m_model->item(i, 2)->text();
			m_data.comments[i].authorUri = m_model->item(i, 3)->text();
		}
	}

	for (int i = 0; i < m_fileList.count(); i++) {
		saveData(m_fileList.at(i));
	}
	QDialog::accept();
}

