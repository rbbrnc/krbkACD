#ifndef METADATA_EDIT_H
#define METADATA_EDIT_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>
#include <QSortFilterProxyModel>

#include "QExiv2.h"

namespace Ui {
	class MetadataEdit;
}

class MetadataEdit : public QWidget
{
	Q_OBJECT

	public:
		explicit MetadataEdit(const QString file, QWidget *parent = 0);
		~MetadataEdit();

	private slots:
		void on_listView_doubleClicked(const QModelIndex &index);
		void addSubjectTag();
		void removeSubjectTag();
		void on_ratingSpinBox_valueChanged(double val);
		void descriptionChanged();
		void imageCommentChanged();

	        void on_saveButton_clicked();

	private:
		Ui::MetadataEdit *ui;
		QStringListModel *m_model;
		QSortFilterProxyModel *m_filter;

		QExiv2 *exiv2;
		bool m_xmpUpdate;
		bool m_commentUpdate;
};

#endif
