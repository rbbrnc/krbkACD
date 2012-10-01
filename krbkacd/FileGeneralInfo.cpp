#include "FileGeneralInfo.h"
#include "ui_FileGeneralInfo.h"

#include <QtGui>

FileGeneralInfo::FileGeneralInfo(const QFileInfo &fileInfo, QWidget *parent)
	: QWidget(parent),
	ui(new Ui::FileGeneralInfo)
{
	ui->setupUi(this);

	ui->fileName->setText(fileInfo.fileName());
	ui->filePath->setText(fileInfo.absoluteFilePath());

	QPixmap thumbnail(fileInfo.fileName());
	ui->thumbnail->setPixmap(thumbnail);
	ui->imageDimension->setText(QString::number(thumbnail.width()) + "x" + QString::number(thumbnail.height()));

	ui->fileSize->setText(QString("%1 bytes").arg(fileInfo.size()));

	ui->fileOwner->setText(fileInfo.owner());
	ui->fileGroup->setText(fileInfo.group());

	ui->fileReadPermission->setChecked(fileInfo.isReadable());
        ui->fileWritePermission->setChecked(fileInfo.isWritable());
        ui->fileExecPermission->setChecked(fileInfo.isExecutable());

	ui->fileLastRead->setText(fileInfo.lastRead().toString());
	ui->fileLastModified->setText(fileInfo.lastModified().toString());
}
