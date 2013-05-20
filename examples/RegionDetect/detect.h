#ifndef OBJECT_DETECT_H
#define OBJECT_DETECT_H

#include <QList>
#include <QRect>

#include <opencv/cv.h>
#include <opencv/highgui.h>

class ObjectDetect
{
	public:
		ObjectDetect(const char *haarCascadePath = NULL);
		~ObjectDetect();

		void setSource(const char *src);
		int  sourceWidth() const;
		int  sourceHeight() const;

		int detect(double scale = 1.1, bool fast = true);
		QList<QRect> objects() const;

	private:

		CvHaarClassifierCascade *m_haarCascade;
		IplImage *m_image;
		CvSeq    *m_objects;
};

#endif

