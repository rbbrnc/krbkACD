#ifndef METADATA_DIALOG_H
#define METADATA_DIALOG_H

#include <QStringListModel>
#include <QSortFilterProxyModel>

#include "QExiv2.h"

namespace Ui {
	class MetadataDialog;
}

class MetadataDialog : public QWidget
{
	Q_OBJECT

	public:
		explicit MetadataDialog(QWidget *parent = 0);
		~MetadataDialog();

		void setFile(const QString &file);
		bool save();

	private:
		void updateMetadata();
		void showErrorMessage(const QString &text);

	private slots:

		// Handle xmp.dc.subject Bag
		void xmpDcSubjectDoubleClicked(const QModelIndex &index);
		void addXmpDcSubject();
		void removeXmpDcSubject();

		// For updating LangAlt Tags
		void updateLangAltString(const QString &text);
		void updateLangAltText();

		// For update the image comment
		void updateImageComment();

	private:
		Ui::MetadataDialog *ui;

		bool m_metadataChanged;

		QExiv2 *m_d;

		QStringListModel      *m_xmpDcSubjectModel;
		QSortFilterProxyModel *m_xmpDcSubjectFilter;
};

#endif
