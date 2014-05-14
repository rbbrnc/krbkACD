#ifndef COPYMOVEDIALOG_H
#define COPYMOVEDIALOG_H

#include <QDialog>

namespace Ui {
	class CopyMoveDialog;
}

class CopyMoveDialog : public QDialog
{
	Q_OBJECT

	public:
        CopyMoveDialog(const QString &file, const QString &toPath, int count = 1, QWidget *parent = 0);
//		CopyMoveDialog(const QStringList &files, const QString &toPath, QWidget *parent = 0);
		~CopyMoveDialog();

	private:
		Ui::CopyMoveDialog *ui;
};

#endif // COPYMOVEDIALOG_H
