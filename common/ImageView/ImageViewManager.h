#ifndef IMAGE_VIEW_MANAGER_H
#define IMAGE_VIEW_MANAGER_H

#include <QtGui>

class ImageGraphicsView;
class ImageGraphicsItem;
class RegionGraphicsItem;
class QExiv2;
class XmpRegion;

class ImageViewManager : public QWidget
{
	Q_OBJECT

	public:
		explicit ImageViewManager(QWidget *parent = 0);
		~ImageViewManager();

	private:
		void setImageRegions(const QString &fileName);
		void addRectRegion(XmpRegion &region);

	signals:
		void requestNextFile();
		void requestPreviousFile();

	public slots:
		void setFile(const QString &fileName);
		void setImage(const QPixmap &pixmap);

		void previous();
		void next();

		void showImageRegions(bool show);
		bool saveImageRegions();

	private slots:
		void sceneChanged(const QList<QRectF> &region);
		void enableRegionSelection(bool enable);

		void addRectRegion(const QRectF &rect);
		void removeRectRegion();
		void editRectRegion();

	private:
		ImageGraphicsItem *m_image;
		ImageGraphicsView *m_view;
		QGraphicsScene    *m_scene;

		QExiv2 *m_exiv2;
		bool m_showRegions;

		QHash<RegionGraphicsItem *, XmpRegion> m_regionHash;
		bool m_updateRegion;
};

#endif
