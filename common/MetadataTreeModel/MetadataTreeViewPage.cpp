#include "MetadataTreeViewPage.h"
#include "ui_MetadataTreeViewPage.h"

MetadataTreeViewPage::MetadataTreeViewPage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MetadataTreeViewPage),
	m_model(0)
{
	ui->setupUi(this);

	connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(next()));
	connect(ui->prevButton, SIGNAL(clicked()), this, SLOT(previous()));
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
	ui->fileLabel->setText(file);

	if (m_model) {
		delete m_model;
		m_model = 0;
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
}

void MetadataTreeViewPage::next()
{
	emit requestNextFile();
}

void MetadataTreeViewPage::previous()
{
	emit requestPreviousFile();
}
