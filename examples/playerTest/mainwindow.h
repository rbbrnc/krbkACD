#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

	public:
		explicit MainWindow(const QString &file, QWidget *parent = 0);
		~MainWindow();

	private:
		void quitPlayer();
		void parsePlayerOutputLine(const QString &line);

	private slots:
		void startProcess();
		void pausePlayer();
		void stopPlayer();
		void stepPlayer();
		void seekPlayer(int sec);

		void readStdout();
		void readStderr();

	private:
		Ui::MainWindow *ui;
		QString m_file;

		QProcess *m_process;

		double m_streamPosition;
};

#endif
