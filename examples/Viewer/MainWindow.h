#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "ImageViewManager.h"

class MainWindow : public QWidget
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
		void setFile(const QString &fileName, bool loadMetadata = false);

	private:
		ImageViewManager *ivm;
};
#endif
