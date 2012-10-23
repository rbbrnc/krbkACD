#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

class FileManager;
class MainWindow : public QWidget
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void copy();
		void move();

	private:
		QVBoxLayout *m_mainLayout;
		FileManager *m_manager;

		QPushButton *m_deleteButton;
		QPushButton *m_renameButton;
		QPushButton *m_hiddenButton;
		QPushButton *m_iconButton;
		QPushButton *m_mkdirButton;

		QPushButton *m_nextButton;
		QPushButton *m_prevButton;
		QPushButton *m_copyButton;
		QPushButton *m_moveButton;

};
#endif
