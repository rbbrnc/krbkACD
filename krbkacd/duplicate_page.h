#ifndef DUPLICATE_PAGE_H
#define DUPLICATE_PAGE_H

#include <QWidget>
#include <QMap>

namespace Ui {
	class DuplicatePage;
}

class DuplicatePage : public QWidget
{
	Q_OBJECT

	public:
		explicit DuplicatePage(QWidget *parent = 0);
		~DuplicatePage();

		void setMap(const QMap<QString, QStringList> &map);

	signals:
		void changePage(int);

	public slots:
		void renameFile();
		void deleteFile();

	private slots:
	        void on_backButton_clicked();
	        void on_nextButton_clicked();
	        void on_prevButton_clicked();

	private:
		Ui::DuplicatePage *ui;

		QMap<QString, QStringList> m_dupMap;
		QStringList m_keyList;
		int m_current;
};

#endif // DUPLICATE_PAGE_H
