#ifndef IMAGE_VIEW_MANAGER_H
#define IMAGE_VIEW_MANAGER_H

#include <QtGui>

class ImageGraphicsView;
class ImageGraphicsItem;

class ImageViewManager : public QWidget
{
	Q_OBJECT

	public:
		explicit ImageViewManager(QWidget *parent = 0);
		~ImageViewManager();

		QString currentFile() const;
		QList<QRectF> rectRegions() const;

	signals:
		void requestNextFile();
		void requestPreviousFile();

	public slots:
		void setFile(const QString &file);
		void setFiles(const QStringList &files);
		void setImage(const QPixmap &pixmap);

		void previous();
		void next();

		void addRectRegion(const QRectF &region);
		void addRectRegions(const QList<QRectF> regions);
		void showImageRegions(bool show);

	private slots:
		void sceneChanged(const QList<QRectF> &region);
		void enableRegionSelection(bool enable);

	private:
		void updateButtons();

	private:
		ImageGraphicsItem *m_image;
		ImageGraphicsView *m_view;

		QGraphicsScene *m_scene;

		qreal m_angle;

		int m_currentFile;
		QStringList m_fileList;
		QList<QGraphicsRectItem *> m_regionList;

		QToolButton *previousButton;
		QToolButton *nextButton;
};

#endif
