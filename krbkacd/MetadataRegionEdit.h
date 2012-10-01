#ifndef METADATA_REGION_EDIT_H
#define METADATA_REGION_EDIT_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QRubberBand>
#include <QPoint>
#include <QStringListModel>

#include "QExiv2.h"

namespace Ui {
	class MetadataRegionEdit;
}

class MetadataRegionEdit : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MetadataRegionEdit(QExiv2 *metadata, QWidget *parent = 0);
		~MetadataRegionEdit();

	private slots:
	        void on_saveButton_clicked();
		void on_cancelButton_clicked();

		void updatePage(const QModelIndex &index);

	        void on_addRegion_clicked();
	        void on_removeRegion_clicked();

	private:
		void mousePressEvent(QMouseEvent *);
		void mouseMoveEvent(QMouseEvent *);
		void mouseReleaseEvent(QMouseEvent *);

		void beginAddRegion();
		void endAddRegion();

	private:
		Ui::MetadataRegionEdit *ui;
		QExiv2 *m_metadata;

		bool m_update;
		bool m_add;

		QStringListModel *m_model;

                QList<PTag> m_tagList;

		QRubberBand *m_rubberBand;
		QPoint mypoint;
};

#endif
