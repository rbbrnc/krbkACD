#ifndef SOCIAL_METADATA_DIALOG_H
#define SOCIAL_METADATA_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "md_social.h"

class QExiv2;

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

        void enableWidgets(bool enable);
		void fillCommentTable();

	private slots:
		void on_addCommentButton_clicked();
		void on_removeCommentButton_clicked();

	public slots:
		virtual void accept();
		virtual void reject();

	private:
		Ui::SocialMetadataDialog *ui;

		QStringList m_fileList;
		SocialMetadata m_data;

		QStandardItemModel *m_model;

        QExiv2 *m_exiv2;
		bool m_writeComments;
};

#endif
