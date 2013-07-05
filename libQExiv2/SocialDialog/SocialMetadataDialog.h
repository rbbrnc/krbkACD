#ifndef SOCIAL_METADATA_DIALOG_H
#define SOCIAL_METADATA_DIALOG_H

#include <QDialog>

class SocialMetadata
{
	public:
		SocialMetadata() {}
		~SocialMetadata() {}

		bool isEmpty() const;

		struct comment {
			QString date;
			QString author;
			QString authorUri;
			QString comment;
		};

		QString albumTitle;
		QString albumDescription;
		QString publisherName;
		QString publisherUri;
		QList<struct comment> comments;
};


namespace Ui {
	class SocialMetadataDialog;
}

class SocialMetadataDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit SocialMetadataDialog(const QStringList &files, QWidget *parent = 0);
		~SocialMetadataDialog();

	private:
		bool loadData(const QString &file);
		bool saveData(const QString &file);

	public slots:
		virtual void accept();
		virtual void reject();

	private:
		Ui::SocialMetadataDialog *ui;

		QStringList m_fileList;
		SocialMetadata m_data;
};

#endif
