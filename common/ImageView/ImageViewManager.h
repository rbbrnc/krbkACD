#ifndef IMAGE_VIEW_MANAGER_H
#define IMAGE_VIEW_MANAGER_H

#include <QtGui>

class ImageGraphicsView;
class ImageGraphicsItem;
class RegionGraphicsItem;

class ImageViewManager : public QWidget
{
	Q_OBJECT

	public:
		explicit ImageViewManager(QWidget *parent = 0);
		~ImageViewManager();

		void insertRegion(const QRectF &rect,
				  const QString &name, const QString &desc);

	signals:
		void requestNextFile();
		void requestPreviousFile();
		void requestDetectObjects();

	public slots:
		void setImage(const QString &fileName);
		void setImage(const QPixmap &pixmap);

		void showRegions(bool show);

	private slots:
		void sceneChanged(const QList<QRectF> &region);
		void enableRegionSelection(bool enable);

		void addRegion(const QRectF &rect);
		void removeRegion();
		void editRegion();

		void previous();
		void next();
		void onDetectObjects();

	private:
		ImageGraphicsItem *m_image;
		ImageGraphicsView *m_view;
		QGraphicsScene    *m_scene;

		QSet<RegionGraphicsItem *> m_regions;

		bool m_showRegions;
		bool m_updateRegion;
};

#endif
