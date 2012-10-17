#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QDir>
#include <QItemSelectionModel>
#include <QKeyEvent>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void on_prevButton_clicked();
		void on_nextButton_clicked();

		void fileSelect(const QModelIndex &current, const QModelIndex &previous);
		void handleItemActivation(QModelIndex index);

		void on_pushButton_clicked();

	public slots:
		void changePath(const QString &path);
		void iconMode(bool enable);

	protected:
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);

	private:
		Ui::MainWindow *ui;

		QFileSystemModel *m_model;
		QDir m_currentDir;
		QModelIndexList m_selection;
		QItemSelectionModel *m_selectionModel;
		QModelIndex m_currentIndex;
};
#endif
