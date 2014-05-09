#include "mainwindow.h"
#include "ui_mainwindow.h"

#define PAGE_CONTACTS   0

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionContacts_triggered()
{
	ui->stackedWidget->setCurrentIndex(PAGE_CONTACTS);
}
