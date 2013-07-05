#ifndef IMAGE_VIEW_MANAGER_H
#define IMAGE_VIEW_MANAGER_H

#include <QtGui>

class QExiv2;
class ImageGraphicsView;
class ImageGraphicsItem;
class RegionGraphicsItem;

class ImageViewManager : public QWidget
{
	Q_OBJECT

	public:
		explicit ImageViewManager(QWidget *parent = 0);
		~ImageViewManager();

		void saveMetadata();

	private:
		void setImage(const QPixmap &pixmap);

		void insertRegion(const QRectF &rect,
				  const QString &name,
				  const QString &desc,
				  int type = 0);
	public slots:
		void setImage(const QString &fileName, bool loadMetadata);
		void addRegion(const QRectF &rect);

	private slots:
		void sceneChanged(const QList<QRectF> &region);
		void enableRegionSelection(bool enable);

		void removeRegion();
		void editRegion();

		void onDetectObjects();
		void showRegions(bool show);

	private:
		ImageGraphicsItem *m_image;
		ImageGraphicsView *m_view;
		QGraphicsScene    *m_scene;

		QExiv2 *m_exiv2;

		QSet<RegionGraphicsItem *> m_regions;

		bool m_showRegions;
		bool m_updateRegion;

		QToolButton *m_zoom11Button;
		QToolButton *m_zoomInButton;
		QToolButton *m_zoomOutButton;
		QToolButton *m_zoomToFitButton;

		QToolButton *m_rotateCCWButton;
		QToolButton *m_rotateCWButton;
		QToolButton *m_resetViewButton;

		QToolButton *m_detectButton;
		QToolButton *m_modeButton;

		QString m_file;
};

#endif
