#ifndef SINGLEFILERENAMEDIALOG_H
#define SINGLEFILERENAMEDIALOG_H

#include <QDialog>

namespace Ui {
class SingleFileRenameDialog;
}

class SingleFileRenameDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit SingleFileRenameDialog(const QString &file, QWidget *parent = 0);
		~SingleFileRenameDialog();

	public slots:
		virtual void accept();

	private:
		bool renameFile();

	private:
		Ui::SingleFileRenameDialog *ui;
		QString m_oldName;
};

#endif // SINGLEFILERENAMEDIALOG_H
