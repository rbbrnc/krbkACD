#ifndef QMAGIC_H
#define QMAGIC_H

class QString;

namespace QMagic {

// MIME type using libmagic
QString mimeType(const QString &filePath);
QString mimeDescription(const QString &filePath);

}
#endif
