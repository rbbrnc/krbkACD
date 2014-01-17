#ifndef BATCH_RENAME_DIALOG_H
#define BATCH_RENAME_DIALOG_H

#include "qglobal.h"
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include <QDialog>

namespace Ui {
	class BatchRenameDialog;
}

class PatternWidget;

class BatchRenameDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit BatchRenameDialog(const QStringList &files, QWidget *parent = 0);
		~BatchRenameDialog();

		QStringList newFileNames();

		// Unused
		//QStringList newFilePaths();

	private:
		void updateNames();

	public slots:
		virtual void accept();
		virtual void reject();

	private slots:
		void addPattern();
		void removePattern();
		void patternChanged(const QString &);

		void fileExtensionUpdate(int index);
		void fileExtensionChanged(const QString &text);

	private:
		Ui::BatchRenameDialog *ui;

		QVBoxLayout *m_patternLayout;

		QList<PatternWidget *> m_patternList;
		QMap<PatternWidget *, QString> m_patternMap;

		QString m_originalPath;

		QStringList m_originalName;
		QStringList m_originalExt;

		QStringList m_newName;
		QStringList m_newExt;
};

#endif
