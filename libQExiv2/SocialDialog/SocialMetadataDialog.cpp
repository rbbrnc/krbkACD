#include "SocialMetadataDialog.h"
#include "ui_SocialMetadataDialog.h"

#include "QExiv2.h"

bool SocialMetadata::isEmpty() const
{
	return false;
/*	return (worldRegion.isEmpty()
		&& countryName.isEmpty()
		&& countryCode.isEmpty()
		&& provinceState.isEmpty()
		&& city.isEmpty()
	        && sublocation.isEmpty());
*/
}

SocialMetadataDialog::SocialMetadataDialog(const QStringList &files, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SocialMetadataDialog),
	m_fileList(files)
{
	ui->setupUi(this);

	if (m_fileList.count() == 0) {
		//"Empty selection!"
	} else if (m_fileList.count() == 1) {
		if (loadData(m_fileList.at(0))) {
			ui->albumTitle->setText(m_data.albumTitle);
			ui->albumDescription->setPlainText(m_data.albumDescription);
			ui->publisherName->setText(m_data.publisherName);
			ui->publisherUri->setText(m_data.publisherUri);
			if (m_data.comments.count() > 0) {
				for (int i = 0; i < m_data.comments.count(); i++) {
					qDebug() << m_data.comments.at(i).date << ":"
						 << m_data.comments.at(i).author << ":"
						 << m_data.comments.at(i).authorUri << ":\n"
						 << m_data.comments.at(i).comment;
				}
			}
		}
	} else {
		// Multiple Selection;
	}

//	ui->label->setTextFormat(Qt::RichText);
//	ui->label->setTextInteractionFlags(Qt::TextBrowserInteraction);
//	ui->label->setOpenExternalLinks(true);
//	ui->label->setText("<a href=\"http://www.google.com/\">Google</a>");
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

	e->setXmpTagString("Xmp.social.PublisherName", m_data.publisherName);
	e->setXmpTagString("Xmp.social.PublisherUri", m_data.publisherUri);
	e->setXmpTagString("Xmp.social.AlbumTitle", m_data.albumTitle);
	e->setXmpTagString("Xmp.social.AlbumDescription", m_data.albumDescription);
	e->setXmpTagBag("Xmp.social.Comments");
	for (int i = 0; i < m_data.comments.count(); i++) {
		e->setXmpTagString(QString("Xmp.social.Comments[%1]/social:CommentDate").arg(i+1), m_data.comments.at(i).date);
		e->setXmpTagString(QString("Xmp.social.Comments[%1]/social:CommentAuthor").arg(i+1),  m_data.comments.at(i).author);
		e->setXmpTagString(QString("Xmp.social.Comments[%1]/social:CommentAuthorUri").arg(i+1), m_data.comments.at(i).authorUri);
		e->setXmpTagString(QString("Xmp.social.Comments[%1]/social:CommentText").arg(i+1), m_data.comments.at(i).comment);
	}

	if (e->save()) {
		delete e;
		return true;
	}

	qDebug() << __PRETTY_FUNCTION__ << "Error set Xmp Location on file:" << file;
	delete e;
	return false;
}

SocialMetadataDialog::~SocialMetadataDialog()
{
	delete ui;
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

	for (int i = 0; i < m_fileList.count(); i++) {
		saveData(m_fileList.at(i));
	}
	QDialog::accept();
}

