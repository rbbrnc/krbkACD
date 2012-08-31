#include <QtGui>

#include "MetadataEdit.h"
#include "MetadataRegionEdit.h"
#include "FileGeneralInfo.h"
#include "MetadataDialog.h"

MetadataDialog::MetadataDialog(const QString &fileName, QWidget *parent)
	: QDialog(parent)
{
	QFileInfo fileInfo(fileName);

	tabWidget = new QTabWidget;
	tabWidget->addTab(new FileGeneralInfo(fileInfo), tr("General"));
	tabWidget->addTab(new MetadataEdit(fileName), tr("Metadata"));
	tabWidget->addTab(new MetadataRegionEdit(fileName), tr("Regions"));

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tabWidget);
	mainLayout->addWidget(buttonBox);
	setLayout(mainLayout);

	setWindowTitle(tr("Tab Dialog"));
}
