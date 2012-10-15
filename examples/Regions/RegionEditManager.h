#ifndef REGION_EDIT_MANAGER_H
#define REGION_EDIT_MANAGER_H

#include <QtGui>

class ImageGraphicsView;
class ImageGraphicsItem;
class QExiv2;

class RegionEditManager : public QWidget
{
	Q_OBJECT

	public:
		explicit RegionEditManager(QWidget *parent = 0);
		~RegionEditManager();

		QString currentFile() const;

	public slots:
		void setFile(const QString &file);
		void setFiles(const QStringList &files);

		void showImageRegions(bool show);

	private slots:
		void sceneChanged(const QList<QRectF> &region);

	private:
		void setImage(const QPixmap &pixmap);

	private:
		ImageGraphicsItem *m_image;
		ImageGraphicsView *m_view;

		QGraphicsScene *m_scene;

		qreal m_angle;

		int m_currentFile;
		QStringList m_fileList;

		QExiv2 *m_metadata;
};

#endif
