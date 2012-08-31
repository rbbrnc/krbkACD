#ifndef FILE_GENERAL_INFO_H
#define FILE_GENERAL_INFO_H

#include <QWidget>
#include <QFileInfo>

namespace Ui {
	class FileGeneralInfo;
}

class FileGeneralInfo : public QWidget
{
	Q_OBJECT

	public:
		FileGeneralInfo(const QFileInfo &fileInfo, QWidget *parent = 0);

	private:
		Ui::FileGeneralInfo *ui;
};

#endif
