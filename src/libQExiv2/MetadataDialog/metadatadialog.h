#ifndef METADATA_DIALOG_H
#define METADATA_DIALOG_H

#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QDialog>

namespace Ui {
	class MetadataDialog;
}

class MetadataDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit MetadataDialog(const QStringList &files, QWidget *parent = 0);
		~MetadataDialog();

	private:
		bool loadData(const QString &file);
		bool saveData(const QString &file);

	public slots:
		virtual void accept();
		virtual void reject();

	private slots:
		// Handle xmp.dc.subject Bag
		void on_xmpDcSubjectListView_doubleClicked(const QModelIndex &index);

		void on_addXmpDcSubjectButton_clicked();
		void on_removeXmpDcSubjectButton_clicked();
		void on_ratingSpinBox_valueChanged(double);

	private:
		Ui::MetadataDialog *ui;

		QStringListModel      *m_xmpDcSubjectModel;
		QSortFilterProxyModel *m_xmpDcSubjectFilter;

		QStringList m_fileList;

		bool m_updateRating;
};

#endif
