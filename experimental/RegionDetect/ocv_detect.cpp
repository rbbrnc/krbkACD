// g++ -Wall ocv_detect.cpp `pkg-config opencv --cflags` -o ocv3.bin `pkg-config opencv --libs`
#include <stdio.h>
#include <cv.h>
#include <highgui.h>

#define HAAR_CASCADE_PATH "/opt/opencv/share/OpenCV/haarcascades/haarcascade_frontalface_alt_tree.xml"

static int detect(char *file)
{
	IplImage* img;
	img = cvLoadImage(file);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvHaarClassifierCascade *cascade = (CvHaarClassifierCascade *) cvLoad(HAAR_CASCADE_PATH);
	double scale = 1.3;

	static CvScalar colors[] = {
		{{0,0,255}}, {{0,128,255}}, {{0,255,255}}, {{0,255,0}}, {{255,128,0}}, {{255,255,0}}, {{255,0,0}}, {{255,0,255}}
	};

	// Detect objects
	cvClearMemStorage(storage);
	CvSeq* objects = cvHaarDetectObjects(img, cascade, storage, 1.1, 4, 0, cvSize(40, 50));

	CvRect* r;
	// Loop through objects and draw boxes
	for (int i = 0; i < (objects ? objects->total : 0 ); i++ ) {
		r = (CvRect *) cvGetSeqElem(objects, i);
		printf("x:%d y:%d w:%d h:%d\n", r->x, r->y, r->width, r->height);
		cvRectangle(img, cvPoint(r->x, r->y), cvPoint(r->x + r->width, r->y + r->height), colors[i%8]);
	}

	cvNamedWindow( "Output" );
	cvShowImage( "Output", img );
	cvWaitKey();
	cvReleaseImage(&img);
	return 0;
}

int main(int argc, char ** argv)
{
	if (argc < 2) {
		printf("missing arg\n");
		return -1;
	}

	return detect(argv[1]);
}

#if 0

// the individual channels for the IplImage
tchannel0 = cvCreateImage(frame_size, IPL_DEPTH_8U, 1);
tchannel1 = cvCreateImage(frame_size, IPL_DEPTH_8U, 1);
tchannel2 = cvCreateImage(frame_size, IPL_DEPTH_8U, 1);
tchannel3 = cvCreateImage(frame_size, IPL_DEPTH_8U, 1);

// set all elements in tchannel0 (alpha channel) to 255
cvSet(tchannel0,cvScalarAll(255),0);

// with cframe being the captured frame (3 channel RGB)
// and dframe the frame to be displayed
cvSplit(cframe, tchannel1, tchannel2, tchannel3, NULL);
cvMerge(tchannel1, tchannel2, tchannel3, tchannel0, dframe);

// point to the image data stored in the IplImage*
const unsigned char * data = (unsigned char *)(dframe->imageData);

// read other parameters in local variables
int width = ocv_image->width;
int height = ocv_image->height;
int bytesPerLine = ocv_image->widthStep;

// imageframe is my QLabel object
qimage = QImage(data, width, height, bytesPerLine, QImage::Format_RGB32);
ui.imageframe->setPixmap(pixmap->fromImage(qimage, 0));
#endif
