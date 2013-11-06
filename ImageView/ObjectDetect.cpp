#include <QDebug>

#include "ObjectDetect.h"

// Default (Fallback) Haar Cascade xml
#ifndef OPENCV_HAARCASCADE
#define HAARCASCADE "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt_tree.xml"
#endif

static CvMemStorage *storage;

ObjectDetect::ObjectDetect(const char *haarCascadePath)
{
	m_objects = NULL;
	m_image = NULL;
	storage = cvCreateMemStorage(0);

	if (!haarCascadePath) {
		m_haarCascade = (CvHaarClassifierCascade *) cvLoad(HAARCASCADE);
	} else {
		m_haarCascade = (CvHaarClassifierCascade *) cvLoad(haarCascadePath);
	}
}

ObjectDetect::~ObjectDetect()
{
	if (m_image) {
		cvReleaseImage(&m_image);
	}
}

void ObjectDetect::setSource(const char *src)
{
	if (m_image) {
		cvReleaseImage(&m_image);
	}

	if (src) {
		// Load image from file
		m_image = cvLoadImage(src);
	}
}

int ObjectDetect::sourceWidth() const
{
	if (m_image) {
		return m_image->width;
	}
	return 0;
}

int ObjectDetect::sourceHeight() const
{
	if (m_image) {
		return m_image->height;
	}
	return 0;
}

// CvSeq* cvHaarDetectObjects(const CvArr* image,
//		CvHaarClassifierCascade *cascade,
//		CvMemStorage* storage,
//		double scale_factor=1.1,
//		int min_neighbors=3,
//		int flags=0,
//		CvSize min_size=cvSize(0,0),
//		CvSize max_size=cvSize(0,0))
int ObjectDetect::detect(double scale, bool fast)
{
	// Detect objects
	cvClearMemStorage(storage);
	if (fast) {
		m_objects = cvHaarDetectObjects(m_image, m_haarCascade, storage, scale, 4, CV_HAAR_DO_CANNY_PRUNING, cvSize(40, 50));
	} else {
		m_objects = cvHaarDetectObjects(m_image, m_haarCascade, storage);
	}

	return (m_objects) ? m_objects->total : 0;
}


QList<QRectF> ObjectDetect::objects() const
{
	QList<QRectF> rects;

	if (m_objects) {
		CvRect *r;
		for (int i = 0; i < m_objects->total; i++ ) {
			r = (CvRect *) cvGetSeqElem(m_objects, i);
			rects.append(QRectF(r->x, r->y, r->width, r->height));
			//printf("x:%d y:%d w:%d h:%d\n", r->x, r->y, r->width, r->height);
		}
	}

	return rects;
}

