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

private:
		QtVariantProperty *testTypes(QtVariantPropertyManager *manager, struct exifData *data);

	private:
		Ui::MetadataEditorPage *ui;

		QtVariantPropertyManager *variantManager;
		QtVariantEditorFactory *variantFactory;

		QtProperty *exifProperties;
		QtProperty *iptcProperties;
		QtProperty *xmpProperties;
		QtProperty *commentProperties;

	        QExiv2 m_metadata;
};

#endif // METADATA_EDITOR_PAGE_H
