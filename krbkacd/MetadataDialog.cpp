#include <QtGui>
#include <QDebug>

#include "MetadataEdit.h"
#include "MetadataRegionEdit.h"
#include "MetadataLocation.h"
#include "FileGeneralInfo.h"
#include "MetadataDialog.h"
#include "QExiv2.h"

MetadataDialog::MetadataDialog(const QString &fileName, QExiv2 *metadata,  QWidget *parent)
	: QDialog(parent)
{
	QFileInfo fileInfo(fileName);

	tabWidget = new QTabWidget;

	m_metadata = 0;
	if (metadata) {
		if (metadata->isValid()) {
			m_metadata = metadata;
		}
	}

	if (m_metadata) {
		tabWidget->addTab(new MetadataEdit(m_metadata), tr("Metadata"));
		tabWidget->addTab(new MetadataRegionEdit(m_metadata), tr("Regions"));
		tabWidget->addTab(new MetadataLocation(m_metadata), tr("Locations"));
	}

	tabWidget->addTab(new FileGeneralInfo(fileInfo), tr("General"));

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tabWidget);
	mainLayout->addWidget(buttonBox);
	setLayout(mainLayout);

	setWindowTitle(tr("Tab Dialog"));
}

void MetadataDialog::accept()
{
	if (m_metadata) {
		m_metadata->save();
	}

	QDialog::accept();
}

