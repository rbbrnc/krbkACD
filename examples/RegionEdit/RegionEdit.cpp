#include "RegionEdit.h"
#include "mwgRegionInfo.h"

#include <QDebug>

/*
Xmp.mwg-rs.Regions Regions RegionInfo XmpText Internal Main structure containing region based information 
Xmp.mwg-rs.AppliedToDimensions AppliedToDimensions Dimensions XmpText External Width and height of image when storing region data 
Xmp.mwg-rs.RegionList RegionList bag RegionStruct XmpBag External List of Region structures
Xmp.mwg-rs.Area Area Area XmpText External Descriptive markers of catalog items by content
Xmp.mwg-rs.Type Type closed Choice of Text XmpText External Type purpose of region (Face|Pet|Focus|BarCode)
Xmp.mwg-rs.Name Name Text XmpText External Name/ short description of content in image region
Xmp.mwg-rs.Description Description     Text    XmpText         External        Usage scenario for a given focus area (EvaluatedUsed|EvaluatedNotUsed|NotEvaluatedNotUsed) 
Xmp.mwg-rs.FocusUsage FocusUsage      closed Choice of Text   XmpText         External        Descriptive markers of catalog items by content
Xmp.mwg-rs.BarCodeValue BarCodeValue    Text    XmpText         External        Decoded BarCode value string
Xmp.mwg-rs.Extensions Extensions      Text    XmpText         Internal        Any top level XMP property to describe the region content


Xmp.mwg-rs.Regions
	mwg-rs:AppliedToDimensions
		stDim:w 640
		stDim:h 400
		stDim:unit pixel
	mwg-rs:RegionList ''
		mwg-rs:RegionList[1]
			mwg-rs:Name Mister Rectangle" empty.jpg
			mwg-rs:Type Face" empty.jpg
			mwg-rs:Area
				stArea:x 0.275312
				stArea:y 0.3775
				stArea:w 0.164375
				stArea:h 0.28125
				stArea:unit normalized

set Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:w 640
set Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:h 400
set Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:unit pixel
set Xmp.mwg-rs.Regions/mwg-rs:RegionList ''
set Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Name Mister Rectangle
set Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Type Face
set Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:x 0.275312
set Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:y 0.3775
set Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:w 0.164375
set Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:h 0.28125
set Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:unit normalized

*/

Widget::Widget(QWidget *parent)
	: QWidget(parent)
{
	m_mainLayout = new QVBoxLayout;

	QPushButton *okButton = new QPushButton("Ok", this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(ok()));

	QLabel *l0 = new QLabel("Dim W,H:");
	m_dimEdit  = new QLineEdit();
	QLabel *l1 = new QLabel("X,Y,W,H:");
	m_posEdit  = new QLineEdit();
	QLabel *l2 = new QLabel("Name:");
	m_nameEdit = new QLineEdit();
	QLabel *l3 = new QLabel("Description:");
	m_descEdit = new QLineEdit();

	m_typeComboBox = new QComboBox();
	m_typeComboBox->insertItems(0, QStringList() << "Face" << "Pet" << "Focus" << "BarCode");

	m_mainLayout->addWidget(l0);
	m_mainLayout->addWidget(m_dimEdit);
	m_mainLayout->addWidget(l1);
	m_mainLayout->addWidget(m_posEdit);
	m_mainLayout->addWidget(l2);
	m_mainLayout->addWidget(m_nameEdit);
	m_mainLayout->addWidget(l3);
	m_mainLayout->addWidget(m_descEdit);
	m_mainLayout->addWidget(m_typeComboBox);
	m_mainLayout->addWidget(okButton);

	setLayout(m_mainLayout);

	m_dimension = QSizeF(640, 480);
	m_area.setRect(0.275312, 0.3775, 0.164375, 0.28125);
	m_tagCount = 0;
}

Widget::~Widget()
{
}


bool setXmpTagString(const QString &s1, const QString &s2)
{
	qDebug() << s1 << " " << s2;
	return true;
}


void Widget::ok()
{

	// Validate inputs
	QString name = m_nameEdit->text();
	if (name.isEmpty()) {
		return;
	}

	QString desc = m_descEdit->text();
	QString type = m_typeComboBox->currentText();

#if 0
	/* For file with no tags set these tags */
	if (m_tagCount == 0) {
		QString mwgDimension = "Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:";
		/*bool*/ setXmpTagString(mwgDimension + "w", QString::number(m_dimension.width()));
		/*bool*/ setXmpTagString(mwgDimension + "h", QString::number(m_dimension.height()));
		/*bool*/ setXmpTagString(mwgDimension + "unit", "pixel");
		/*bool*/ setXmpTagString("Xmp.mwg-rs.Regions/mwg-rs:RegionList", "");
	}

	m_tagCount++;
	QString mwgRegion = QString("Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:").arg(QString::number(m_tagCount));
	/*bool*/ setXmpTagString(mwgRegion + "Name", name);
	/*bool*/ setXmpTagString(mwgRegion + "Description", desc);
	/*bool*/ setXmpTagString(mwgRegion + "Type", type);


	QString mwgArea = QString("Xmp.mwg-rs.Regions/mwg-rs:RegionList[%1]/mwg-rs:Area/stArea:").arg(QString::number(m_tagCount));

	/*bool*/ setXmpTagString(mwgArea + "x", QString::number(m_area.x()));
	/*bool*/ setXmpTagString(mwgArea + "y", QString::number(m_area.y()));
	/*bool*/ setXmpTagString(mwgArea + "w", QString::number(m_area.width()));
	/*bool*/ setXmpTagString(mwgArea + "h", QString::number(m_area.height()));
	/*bool*/ setXmpTagString(mwgArea + "unit", "normalized");
#endif

	MwgRegionInfo ri;
	ri.insertRegion(QSizeF(800, 600), QRectF(0, 0, 320, 240), name);
	ri.save();
}

