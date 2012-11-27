#ifndef AV_MANAGER_H
#define AV_MANAGER_H

#include <QWidget>

#include "VideoDecode.h"

namespace Ui {
	class AvManager;
}

class AvManager : public QWidget
{
	Q_OBJECT

	public:
		AvManager(QWidget *parent = 0);
		AvManager(const QString &fileName, QWidget *parent = 0);

		~AvManager();

	private:
		void updateTimeLabel(qint64 time);

	private slots:
		void updateFrame(qint64 time);
		void playVideo();
		void ffwdVideo();
		void rewVideo();
		void seekFrame();

	private:
		Ui::AvManager *ui;

		QPixmap m_pic;

		VideoDecode *m_avThread;

		qint64 m_duration;
};

#endif