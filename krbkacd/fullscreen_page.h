#ifndef FULLSCREEN_PAGE_H
#define FULLSCREEN_PAGE_H

#include <QWidget>

namespace Ui {
	class FullScreenPage;
}

class FullScreenPage : public QWidget
{
	Q_OBJECT

	public:
		explicit FullScreenPage(QWidget *parent = 0);
		~FullScreenPage();

	public slots:
		void showEvent(QShowEvent *);
		void hideEvent(QHideEvent *);
		void setPixmap(QString file);
		void setPixmap(QPixmap pix);

	private:
		Ui::FullScreenPage *ui;
};

#endif // FULLSCREEN_PAGE_H
