#ifndef QMAGIC_H
#define QMAGIC_H

//#include <QMimeData>

namespace QMagic {

// MIME type using libmagic
QString mimeType(const QString &file);
bool mimeTypeIsImage(const QString &file);

// Unused
//QString mimeDescription(const QString &file);
//void mimeData(QMimeData &mimeData, const QString &file);
}
#endif
