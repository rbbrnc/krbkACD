#include <QDebug>
#include <QtGui>

#include "FileManager.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QWidget(parent)
{
	m_mainLayout = new QVBoxLayout;

	m_manager = new FileManager();

	m_deleteButton = new QPushButton("delete");
	m_renameButton = new QPushButton("rename");

	m_hiddenButton = new QPushButton("hidden");
	m_hiddenButton->setCheckable(true);

	m_iconButton = new QPushButton("icon");
	m_iconButton->setCheckable(true);

	m_mkdirButton  = new QPushButton("mkDir");

	m_nextButton  = new QPushButton(">");
	m_prevButton  = new QPushButton("<");

	m_copyButton  = new QPushButton("copy");
	m_moveButton  = new QPushButton("move");

	QPushButton *m_Button  = new QPushButton("BTN");

	connect(m_hiddenButton, SIGNAL(toggled(bool)), m_manager, SLOT(showHidden(bool)));
	connect(m_iconButton,   SIGNAL(toggled(bool)), m_manager, SLOT(iconMode(bool)));

	connect(m_deleteButton, SIGNAL(clicked()), m_manager, SLOT(remove()));
	connect(m_renameButton, SIGNAL(clicked()), m_manager, SLOT(rename()));
	connect(m_mkdirButton,  SIGNAL(clicked()), m_manager, SLOT(mkDir()));
	connect(m_nextButton,   SIGNAL(clicked()), m_manager, SLOT(next()));
	connect(m_prevButton,   SIGNAL(clicked()), m_manager, SLOT(previous()));

	connect(m_copyButton,   SIGNAL(clicked()), this, SLOT(copy()));
	connect(m_moveButton,   SIGNAL(clicked()), this, SLOT(move()));

//	connect(m_Button,   SIGNAL(clicked()), this, SLOT(test()));
	connect(m_manager, SIGNAL(currentChanged(const QString &)),
		this, SLOT(test(const QString &)));

	m_mainLayout->addWidget(m_manager);
	m_mainLayout->addWidget(m_renameButton);
	m_mainLayout->addWidget(m_deleteButton);
	m_mainLayout->addWidget(m_hiddenButton);
	m_mainLayout->addWidget(m_iconButton);
	m_mainLayout->addWidget(m_mkdirButton);
	m_mainLayout->addWidget(m_copyButton);
	m_mainLayout->addWidget(m_moveButton);
	m_mainLayout->addWidget(m_prevButton);
	m_mainLayout->addWidget(m_nextButton);
	m_mainLayout->addWidget(m_Button);

	setLayout(m_mainLayout);
}

MainWindow::~MainWindow()
{
	delete m_deleteButton;
	delete m_renameButton;
	delete m_hiddenButton;
	delete m_iconButton;
	delete m_mkdirButton;

	delete m_manager;
}

void MainWindow::copy()
{
	m_manager->copy(".");
}

void MainWindow::move()
{
	m_manager->move(".");
}

void MainWindow::test(const QString &str)
{
	//qDebug() << m_manager->currentPath() << m_manager->currentFile();
	qDebug() << str;
}


