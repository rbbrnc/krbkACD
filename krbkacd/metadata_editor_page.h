#ifndef METADATA_EDITOR_PAGE_H
#define METADATA_EDITOR_PAGE_H

#include <QWidget>

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

#include "QExiv2.h"
#include "file_data.h"

namespace Ui {
	class MetadataEditorPage;
}

class MetadataEditorPage : public QWidget
{
	Q_OBJECT

	public:
		explicit MetadataEditorPage(QWidget *parent = 0);
		 ~MetadataEditorPage();

		void setFileData(FileData fdata);

	private slots:
	        void on_cancelButton_clicked();
		void on_updateButton_clicked();
		void propertyValueChanged(QtProperty *prop, const QVariant &val);
		void slotCurrentItemChanged(QtBrowserItem *currentItem);

	private:
		QtVariantProperty *testTypes(QtVariantPropertyManager *manager, struct exifData *data, bool readOnly);
		void clear();

	private:
		Ui::MetadataEditorPage *ui;

		QtVariantPropertyManager *m_manager;
		QtVariantEditorFactory   *m_factory;

		QtProperty *m_exifProperties;
		QtProperty *m_iptcProperties;
		QtProperty *m_xmpProperties;
		QtProperty *m_commentProperties;

	        QExiv2 m_metadata;
};

#endif // METADATA_EDITOR_PAGE_H
