#ifndef IMAGE_VIEW_MANAGER_H
#define IMAGE_VIEW_MANAGER_H

#include <QtGui>

class ImageGraphicsView;
class ImageGraphicsItem;
class RegionGraphicsItem;


#ifdef USE_QEXIV2
class QExiv2;
class XmpRegion;
#endif

class ImageViewManager : public QWidget
{
	Q_OBJECT

	public:
		explicit ImageViewManager(QWidget *parent = 0);
		~ImageViewManager();

	public slots:
		void setFile(const QString &fileName);
		void setImage(const QPixmap &pixmap);

		void zoomIn();
		void zoomOut();
		void zoom11();
		void zoomToFit();
		void rotateCCW();
		void rotateCW();
		void resetView();

		void enableRegionSelection(bool enable);
		void addRegion(const QRectF &rect);
		void showRegions(bool show);

	private slots:
		void sceneChanged(const QList<QRectF> &region);
		void removeRegion();
		void editRegion();

	private:
		ImageGraphicsItem *m_image;
		ImageGraphicsView *m_view;
		QGraphicsScene    *m_scene;

		bool m_showRegions;
		bool m_updateRegion;

// For Image Regions
#ifdef USE_QEXIV2
	private:
		void setImageRegions(const QString &fileName);

		QExiv2 *m_exiv2;
		QHash<RegionGraphicsItem *, XmpRegion> m_regionHash;

	public slots:
		bool saveImageRegions();
#endif
};

#endif
