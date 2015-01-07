#ifndef IMAGE_VIEW_MANAGER_H
#define IMAGE_VIEW_MANAGER_H

#include "qglobal.h"
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

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
		void loadMetadata();

	private:
		void insertRegion(const QRectF &rect,
				  const QString &name,
				  const QString &desc,
				  int type = 0);
	public slots:
		void setImage(const QString &fileName, bool withMetadata);
		void addRegion(const QRectF &rect);

	private slots:
		void sceneChanged(const QList<QRectF> &region);
		void enableRegionSelection(bool enable);

		void removeRegion();
		void editRegion();

#ifdef ENABLE_OBJECT_DETECT_MODULE
		void onDetectObjects();
#endif
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

#ifdef ENABLE_OBJECT_DETECT_MODULE
		QToolButton *m_detectButton;
#endif
		QToolButton *m_modeButton;

		QString m_file;
};

#endif
