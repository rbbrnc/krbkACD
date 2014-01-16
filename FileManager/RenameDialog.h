#ifndef RENAME_DIALOG_H
#define RENAME_DIALOG_H

#include "qglobal.h"
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

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

		QString newFileName() const;

		// Unused
		//QString newFilePath() const;

	private:
		void updateName();

	public slots:
		virtual void accept();
		virtual void reject();

	private slots:
		void usePattern(bool enable);
		void addPattern();
		void removePattern();
		void patternChanged(const QString &);
		void fileExtensionUpdate(int index);
		void fileExtensionChanged(const QString &text);

	private:
		Ui::RenameDialog *ui;

		QVBoxLayout *m_patternLayout;

		bool m_usePattern;

		QString m_originalPath;
		QString m_originalName;
		QString m_originalExt;

		QString m_newName;
		QString m_newExt;

		QList<PatternWidget *> m_patternList;
		QMap<PatternWidget *, QString> m_patternMap;
};

#endif
