#include "thumb_view.h"

// Global Variable
int mainWidth = 0;
int mainHeight = 0;

int ImageWidth = 0;
int ImageHeight = 0;

// Global Functions

static ThumbInfo makeThumbnail(const QString &filePath)
{
	ImageWidth  = (int)(mainWidth * 15) / 100;
	ImageHeight = (int)(mainHeight * 15) / 100;

	ThumbInfo result;
	QImage image(filePath);

	if (image.isNull())
		qDebug() << "Cannot display" << filePath;

	result.m_thumbnail = image.scaled(QSize(ImageWidth, ImageHeight), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	result.m_filePath  = filePath;
	return result;
}

ThumbView::ThumbView(QWidget *parent)
	: QGraphicsView(parent),
	  m_row(0),
	  m_column(0),
	  m_mouseButtonPressed(false),
	  m_ZoomFactor(1)
{
	m_watcher = new QFutureWatcher<ThumbInfo>(this);

	setScene(&m_graphicsScene);
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
	setCacheMode(QGraphicsView::CacheNone);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	connect(m_watcher, SIGNAL(resultReadyAt(int)), SLOT(showResult(int)));
}

ThumbView::~ThumbView()
{
	delete m_watcher;
}

/* Clear all the items from the view. */
void ThumbView::clear()
{
	m_watcher->cancel();
	m_watcher->waitForFinished();
	m_graphicsScene.clear();

	if (horizontalScrollBar() != NULL) {
		horizontalScrollBar()->setValue(horizontalScrollBar()->minimum());
	}

	m_graphicsScene.setSceneRect(QRectF(0,0,0,0));

	m_row    = 0;
	m_column = 0;
}

void ThumbView::loadImages(QStringList fileList)
{
	clear();
	mainWidth  = geometry().width();
	mainHeight = geometry().height();

	if (fileList.isEmpty()) {
		setMessage("0 Files Found");
	} else {
		m_watcher->setFuture(QtConcurrent::mapped(fileList, makeThumbnail));
	}
}

void ThumbView::showResult(int n)
{
	QGraphicsPixmapItem *imageItem = new QGraphicsPixmapItem();

	ThumbInfo result = m_watcher->resultAt(n);

	imageItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
	imageItem->setPixmap(QPixmap::fromImage(result.m_thumbnail));
	imageItem->setToolTip(result.m_filePath);


	if (m_row >= 4) {
		m_row = 0;
		m_column++;
	}

	QRectF rect = imageItem->boundingRect();
	rect.setWidth(ImageWidth + 10);
	rect.setHeight(ImageHeight + 10);
	imageItem->setPos(m_column * rect.width() * 1.05, m_row * rect.height() * 1.3);
	m_graphicsScene.addItem(imageItem);

	m_graphicsScene.setSceneRect(m_graphicsScene.itemsBoundingRect());

	m_row++;
	m_graphicsScene.update();
}

/* Sets a text message on the view */
void ThumbView::setMessage(const QString &text)
{
	QGraphicsTextItem *textMessage = new QGraphicsTextItem(text);
	textMessage->setFont(QFont("Times", 20));
	textMessage->setDefaultTextColor(QColor(255,255,255,255));
	textMessage->setPos(m_column * textMessage->boundingRect().width() * 1.05, m_row * textMessage->boundingRect().height() * 1.3 );

	m_graphicsScene.addItem(textMessage);
	m_graphicsScene.setSceneRect(m_graphicsScene.itemsBoundingRect());
}

void ThumbView::setZoom(qreal zoomFactor)
{
	m_ZoomFactor += zoomFactor;

	if (zoomFactor > 0) {
		/* max 200% */
		if ((m_ZoomFactor - 2) > 0) {
			m_ZoomFactor = 2;
		}
	} else {
		/* max 25% */
		if ((m_ZoomFactor - 0.25) < 0) {
			m_ZoomFactor = 0.25;
		}
	}

	setTransform(QTransform()
			.translate(contentsRect().width(), contentsRect().height())
                        .scale(m_ZoomFactor, m_ZoomFactor)
                        .translate(-contentsRect().width(), -contentsRect().height()));
}

void ThumbView::wheelEvent(QWheelEvent *e)
{
	int numDegrees = e->delta() / 8;
	int numSteps   = numDegrees / 15;

	if (numSteps > 0) {
		setZoom(0.25);		// Zoom In
	} else if (numSteps < 0) {
		setZoom(-0.25);		// Zoom Out
	}

	QGraphicsView::wheelEvent(e);
}

void ThumbView::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		m_mouseDeltaX = e->x();
		m_mouseButtonPressed = true;
	}

	QGraphicsView::mousePressEvent(e);
}

void ThumbView::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		m_mouseButtonPressed = false;
	}

	QGraphicsView::mouseReleaseEvent(e);
}

void ThumbView::mouseMoveEvent(QMouseEvent *e)
{
	if (m_mouseButtonPressed) {
		int direction = m_mouseDeltaX - e->x();
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() + direction);
		m_mouseDeltaX = e->x();
	}

	QGraphicsView::mouseMoveEvent(e);
}
