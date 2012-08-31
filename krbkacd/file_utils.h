#ifndef FILE_UTILS_H
#define FILE_UTILS_H

void deleteFile(QString filePath, QWidget *parent);
void deleteFiles(QStringList fileList, QWidget *parent);
void renameFile(QString filePath, QWidget *parent);
bool copyFile(const QString destPath, const QString filePath, QWidget *parent);
bool moveFile(const QString destPath, const QString fileName, QWidget *parent);
void makeDir(const QString destPath, QWidget *parent);

// MIME type using libmagic
QString mimeType(const QString &filePath);

// MD5 digest
QByteArray md5(const QString &filePath);
QByteArray md5(const QByteArray &data);

#endif // FILE_UTILS_H
