#include <QtGui>
#include "MainWindow.h"

//#include "QExiv2.h"

void MainWindow::setFile(const QString &fileName, bool loadMetadata)
{
#if 0
	if (loadMetadata) {
		QExiv2 *e = new QExiv2();
		if (m_exiv2->load(fileName)) {
			if (m_exiv2->xmpHasRegionTags()) {
				// Get XMP Image Regions
				//QList<XmpRegion> rl = m_exiv2->xmpRegionList();
				for (int i = 0; i < rl.size(); i++) {
					//rl.at(i).debug();
					//XmpRegion r = rl.at(i);
					//addRectRegion(r);
				}
			}
		}
	}
#endif
	ivm->setFile(fileName);
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
	QSize iconSize(32, 32);

	// Zoom In
	QToolButton *zoomInButton  = new QToolButton();
	zoomInButton->setIcon(QIcon(":/images/zoom_in.png"));
	zoomInButton->setIconSize(iconSize);

	// Zoom Out
	QToolButton *zoomOutButton = new QToolButton();
	zoomOutButton->setIcon(QIcon(":/images/zoom_out.png"));
	zoomOutButton->setIconSize(iconSize);

	// Zoom to Fit
	QToolButton *zoomToFitButton = new QToolButton();
	zoomToFitButton->setIcon(QIcon(":/images/zoom_best_fit.png"));
	zoomToFitButton->setIconSize(iconSize);

	// Zoom 1:1
	QToolButton *zoom11Button = new QToolButton();
	zoom11Button->setIcon(QIcon(":/images/zoom_original.png"));
	zoom11Button->setIconSize(iconSize);

	// Rotate CCW
	QToolButton *rotateCCWButton = new QToolButton();
	rotateCCWButton->setIcon(QIcon(":/images/rotate_ccw.png"));
	rotateCCWButton->setIconSize(iconSize);

	// Rotate CW
	QToolButton *rotateCWButton = new QToolButton();
	rotateCWButton->setIcon(QIcon(":/images/rotate_cw.png"));
	rotateCWButton->setIconSize(iconSize);

	// Reset View to original
	QToolButton *resetViewButton = new QToolButton();
	resetViewButton->setIcon(QIcon(":/images/original.png"));
	resetViewButton->setIconSize(iconSize);

	// View Mode for Panning image or for region selection.
	QToolButton *modeButton = new QToolButton();
	modeButton->setCheckable(true);
	modeButton->setIcon(QIcon(":/images/select.png"));
	modeButton->setIconSize(iconSize);

	// Previous Image
	QToolButton *previousButton = new QToolButton();
	previousButton->setIcon(QIcon(":/images/previous.png"));
	previousButton->setIconSize(iconSize);

	// Next Image
	QToolButton *nextButton = new QToolButton();
	nextButton->setIcon(QIcon(":/images/next.png"));
	nextButton->setIconSize(iconSize);
/*
	// Detect Objects
	QToolButton *detectButton = new QToolButton();
	detectButton->setIcon(QIcon(":/images/next.png"));
	detectButton->setIconSize(iconSize);
*/
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(zoomInButton);
	buttonLayout->addWidget(zoomOutButton);
	buttonLayout->addWidget(zoom11Button);
	buttonLayout->addWidget(zoomToFitButton);
	buttonLayout->addWidget(rotateCCWButton);
	buttonLayout->addWidget(rotateCWButton);
	buttonLayout->addWidget(resetViewButton);
	buttonLayout->addWidget(modeButton);
	buttonLayout->addWidget(previousButton);
	buttonLayout->addWidget(nextButton);
//	buttonLayout->addWidget(detectButton);

	QVBoxLayout *layout = new QVBoxLayout;

	ivm = new ImageViewManager(this);
	ivm->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	layout->addWidget(ivm);
	layout->addLayout(buttonLayout);
	setLayout(layout);


	connect(zoomInButton,    SIGNAL(clicked()), ivm, SLOT(zoomIn()));
	connect(zoomOutButton,   SIGNAL(clicked()), ivm, SLOT(zoomOut()));
	connect(zoom11Button,    SIGNAL(clicked()), ivm, SLOT(zoom11()));
	connect(zoomToFitButton, SIGNAL(clicked()), ivm, SLOT(zoomToFit()));
	connect(rotateCCWButton, SIGNAL(clicked()), ivm, SLOT(rotateCCW()));
	connect(rotateCWButton,  SIGNAL(clicked()), ivm, SLOT(rotateCW()));
	connect(resetViewButton, SIGNAL(clicked()), ivm, SLOT(resetView()));
	connect(modeButton,      SIGNAL(toggled(bool)), ivm, SLOT(enableRegionSelection(bool)));

//	connect(previousButton, SIGNAL(clicked()), ivm, SLOT(previous()));
//	connect(nextButton,     SIGNAL(clicked()), ivm, SLOT(next()));
//	connect(detectButton,   SIGNAL(clicked()), this, SLOT(detectObjects()));
}

MainWindow::~MainWindow()
{

}
