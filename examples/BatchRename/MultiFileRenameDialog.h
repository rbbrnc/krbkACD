#ifndef MULTIFILERENAMEDIALOG_H
#define MULTIFILERENAMEDIALOG_H

#include <QDialog>

namespace Ui {
	class MultiFileRenameDialog;
}

class MultiFileRenameDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit MultiFileRenameDialog(const QStringList &files, QWidget *parent = 0);
		~MultiFileRenameDialog();

	public slots:
		virtual void accept();

private slots:
    void on_addToolButton_clicked();

private:
		bool renameFiles();

	private:
		Ui::MultiFileRenameDialog *ui;

		QStringList m_oldList;
		QStringList m_newList;
};

#endif // MULTIFILERENAMEDIALOG_H
