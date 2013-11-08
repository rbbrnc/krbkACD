#include "MetadataTreeViewPage.h"
#include "ui_MetadataTreeViewPage.h"

#include <QFileInfo>
#include "QExiv2.h"

MetadataTreeViewPage::MetadataTreeViewPage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MetadataTreeViewPage),
	m_model(0)
{
	ui->setupUi(this);
}

MetadataTreeViewPage::~MetadataTreeViewPage()
{
	if (m_model) {
		delete m_model;
		m_model = 0;
	}

	delete ui;
}

void MetadataTreeViewPage::setFile(const QString &file)
{
	if (m_model) {
		delete m_model;
		m_model = 0;
	}

	// Load medatata only for regular files.
	QFileInfo fi(file);
	if (!fi.isFile()) {
		return;
	}

	QExiv2 metadata;

	metadata.load(file);
	if (!metadata.isValid()) {
		return;
	}

	m_model = new MetadataTreeModel(&metadata);
	ui->treeView->setUniformRowHeights(true);
	ui->treeView->setModel(m_model);
	ui->treeView->hideColumn(7); // hide key column
        ui->treeView->expandAll();
}
