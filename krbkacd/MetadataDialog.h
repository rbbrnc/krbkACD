#ifndef METADATA_DIALOG_H
#define METADATA_DIALOG_H

#include <QDialog>

#include <QDialogButtonBox>
#include <QFileInfo>
#include <QTabWidget>

class QExiv2;
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

	public slots:
		virtual void accept();

	private:
		QTabWidget *tabWidget;
		QDialogButtonBox *buttonBox;
		QExiv2 *m_metadata;
};

#endif
