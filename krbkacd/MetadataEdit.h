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
		explicit MetadataEdit(QExiv2 *metadata, QWidget *parent = 0);
		~MetadataEdit();

	private slots:
		void on_listView_doubleClicked(const QModelIndex &index);
		void addSubjectTag();
		void removeSubjectTag();
		void on_ratingSpinBox_valueChanged(double val);
		void imageCommentChanged();

		void xmpEventChanged(const QString &text);
		void xmpTitleChanged(const QString &text);
		void xmpDescriptionChanged();

	private:
		Ui::MetadataEdit *ui;
		QStringListModel *m_model;
		QSortFilterProxyModel *m_filter;
		QExiv2 *m_metadata;
};

#endif
