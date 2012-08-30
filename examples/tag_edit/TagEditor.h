#ifndef TAG_EDITOR_H
#define TAG_EDITOR_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QRubberBand>
#include <QPoint>
#include <QStringListModel>

#include "QExiv2.h"

namespace Ui {
	class TagEditor;
}

class TagEditor : public QMainWindow
{
	Q_OBJECT

	public:
		explicit TagEditor(const QString file, QWidget *parent = 0);
		~TagEditor();

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
		Ui::TagEditor *ui;
		QExiv2 *m_exiv2;

		bool m_update;
		bool m_add;

		QStringListModel *m_model;

                QList<PTag> m_tagList;

		QRubberBand *m_rubberBand;
		QPoint mypoint;
};

#endif // TAG_EDITOR_H
