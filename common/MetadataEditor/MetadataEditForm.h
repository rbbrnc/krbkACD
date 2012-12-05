#ifndef METADATAEDITFORM_H
#define METADATAEDITFORM_H

#include <QWidget>

#include <QStringListModel>
#include <QSortFilterProxyModel>

#include "QExiv2.h"

namespace Ui {
	class MetadataEditForm;
}

class MetadataEditForm : public QWidget
{
	Q_OBJECT

	public:
		explicit MetadataEditForm(QWidget *parent = 0);
		~MetadataEditForm();

		void setFile(const QString &file);
		bool save();

	private:
		void updateMetadata();
		void updateLocations();
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
		Ui::MetadataEditForm *ui;

		bool m_metadataChanged;

		QExiv2 *m_d;

		QStringListModel      *m_xmpDcSubjectModel;
		QSortFilterProxyModel *m_xmpDcSubjectFilter;
};

#endif
