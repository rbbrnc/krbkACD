#ifndef SOCIALMETADATADIALOG_H
#define SOCIALMETADATADIALOG_H

#include <QDialog>

namespace Ui {
	class SocialMetadataDialog;
}

class SocialMetadataDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit SocialMetadataDialog(const QStringList &files, QWidget *parent = 0);
		~SocialMetadataDialog();

	private:
		Ui::SocialMetadataDialog *ui;
};

#endif // SOCIALMETADATADIALOG_H
