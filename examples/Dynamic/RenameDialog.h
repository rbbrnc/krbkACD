#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QtGui>
#include <QDialog>

namespace Ui {
	class RenameDialog;
}

class PatternWidget;
class RenameDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit RenameDialog(const QString &fileName, QWidget *parent = 0);
		~RenameDialog();

		QString newFileName();

	public slots:
		virtual void accept();

	private slots:
		void addPattern();
		void removePattern();

	private:
		Ui::RenameDialog *ui;

		QList<PatternWidget *> m_patternList;
		QString m_originalName;

		QVBoxLayout *m_patternLayout;
};

#endif
