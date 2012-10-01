#ifndef METADATA_DIALOG_H
#define METADATA_DIALOG_H

#include <QDialog>

#include <QDialogButtonBox>
#include <QFileInfo>
#include <QTabWidget>

class QExiv2;

class MetadataDialog : public QDialog
{
	Q_OBJECT

	public:
		MetadataDialog(const QString &fileName, QExiv2 *metadata = 0, QWidget *parent = 0);

	public slots:
		virtual void accept();

	private:
		QTabWidget *tabWidget;
		QDialogButtonBox *buttonBox;
		QExiv2 *m_metadata;
};

#endif
