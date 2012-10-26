#ifndef QMAGIC_H
#define QMAGIC_H

#include <QMimeData>

namespace QMagic {

// MIME type using libmagic
QString mimeType(const QString &file);
QString mimeDescription(const QString &file);
void mimeData(QMimeData &mimeData, const QString &file);
}
#endif
