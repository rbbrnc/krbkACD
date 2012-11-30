#ifndef IMAGE_VIEW_MANAGER_H
#define IMAGE_VIEW_MANAGER_H

#include <QtGui>

class ImageGraphicsView;
class ImageGraphicsItem;
class RegionGraphicsItem;

#define USE_EXIV2

#ifdef USE_EXIV2
class QExiv2;
#endif

class ImageViewManager : public QWidget
{
	Q_OBJECT

	public:
		explicit ImageViewManager(QWidget *parent = 0);
		~ImageViewManager();

		QString currentFile() const;
		QList<QRectF> rectRegions() const;

	private:
		Q_DECL_DEPRECATED void updateButtons();
		void setImageRegions(const QString &fileName);

	signals:
		void requestNextFile();
		void requestPreviousFile();

	public slots:
		void setFile(const QString &fileName);
		Q_DECL_DEPRECATED void setFiles(const QStringList &files);
		void setImage(const QPixmap &pixmap);

		void previous();
		void next();

		void addRectRegion(const QRectF &region, const QString &name = 0, const QString &text = 0, bool normalized = false);
		void showImageRegions(bool show);

	private slots:
		void sceneChanged(const QList<QRectF> &region);
		void sceneSelectionChanged();
		void enableRegionSelection(bool enable);

		void removeRectRegion();
		void editRectRegion();

	private:
		ImageGraphicsItem *m_image;
		ImageGraphicsView *m_view;

		QGraphicsScene *m_scene;

		//qreal m_angle;

		int m_currentFile;
		QStringList m_fileList;
		QList<QGraphicsRectItem *> m_regionList;

		QToolButton *previousButton;
		QToolButton *nextButton;

#ifdef USE_EXIV2
		QExiv2 *m_exiv2;
		bool m_showRegions;
#endif
};

#endif
