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

	public slots:
		void setFile(const QString &file);
		void setFiles(const QStringList &files);
		void setImage(const QPixmap &pixmap);

		void previous();
		void next();

		void showImageRegions(bool show);

	private slots:
		void sceneChanged(const QList<QRectF> &region);

	private:
		void updateButtons();

	private:
		ImageGraphicsItem *m_image;
		ImageGraphicsView *m_view;

		QGraphicsScene *m_scene;

		qreal m_angle;

		int m_currentFile;
		QStringList m_fileList;

		QPushButton *previousButton;
		QPushButton *nextButton;

/*
                PTagGraphicsItem *m_tagItem;
                QList<PTagGraphicsItem *> m_tagItemList;

                Metadata           m_metadata;
                QList<PhotoRegion> m_tagList;

		QString m_currentFile;
		FileData m_fileData;
*/
};

#endif
