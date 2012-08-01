#ifndef DUPLICATE_PAGE_H
#define DUPLICATE_PAGE_H

#include <QWidget>

namespace Ui {
	class DuplicatePage;
}

class DuplicatePage : public QWidget
{
	Q_OBJECT

	public:
		explicit DuplicatePage(QWidget *parent = 0);
		~DuplicatePage();

	signals:
		void changePage(int);

	private slots:
	        void on_backButton_clicked();
	        void on_nextButton_clicked();
	        void on_prevButton_clicked();

	private:
		Ui::DuplicatePage *ui;
};

#endif // DUPLICATE_PAGE_H
