#ifndef FILE_UTILS_H
#define FILE_UTILS_H

void deleteFile(QString filePath, QWidget *parent);
void renameFile(QString filePath, QWidget *parent);
bool copyFile(const QString destPath, const QString filePath, QWidget *parent);
bool moveFile(const QString destPath, const QString fileName, QWidget *parent);
void makeDir(const QString destPath, QWidget *parent);

#endif // FILE_UTILS_H
