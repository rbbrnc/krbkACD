#ifndef METADATA_DIALOG_H
#define METADATA_DIALOG_H

#include <QDialog>

#include <QDialogButtonBox>
#include <QFileInfo>
#include <QTabWidget>

class GeneralTab : public QWidget
{
    Q_OBJECT

public:
    GeneralTab(const QFileInfo &fileInfo, QWidget *parent = 0);
};

class MetadataDialog : public QDialog
{
	Q_OBJECT

	public:
		MetadataDialog(const QString &fileName, QWidget *parent = 0);

	private:
		QTabWidget *tabWidget;
		QDialogButtonBox *buttonBox;
};

#endif
