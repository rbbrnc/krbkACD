#ifndef THUMB_VIEW_H
#define THUMB_VIEW_H

#include <QGraphicsView>
#include <QtGui>

struct ThumbInfo {
	QImage  m_thumbnail;
	QString m_filePath;
};

class ThumbView : public QGraphicsView
{
	Q_OBJECT

	public:
		ThumbView(QWidget *parent = 0);
		virtual ~ThumbView();

	public slots:
		void loadImages(QStringList fileList);
		void setZoom(qreal zoomFactor);

	private slots:
		void showResult(int n);

	protected:
		virtual void wheelEvent(QWheelEvent *);
		virtual void mousePressEvent(QMouseEvent *);
		virtual void mouseReleaseEvent(QMouseEvent *);
		virtual void mouseMoveEvent(QMouseEvent *);

	private:
		void clear();
		void setMessage(const QString &text);

	private:
		QGraphicsScene m_graphicsScene;
		QFutureWatcher<ThumbInfo> *m_watcher;

		int m_row;
		int m_column;

		int   m_mouseDeltaX;
		bool  m_mouseButtonPressed;

		qreal m_ZoomFactor;
};
#endif
