//
// Img Comment Functions
//

#include "QExiv2.h"
#include "QExiv2_p.h"

QByteArray QExiv2::imgComment() const
{
	return QByteArray(d->imageComment.data(), d->imageComment.size());
}

bool QExiv2::setImgComment(const QByteArray& data)
{
	d->imageComment = std::string(data.data(), data.size());
	return true;
}

#if 0
bool QExiv2::clearImgComment()
{
	return setImgComment(QByteArray());
}
#endif

bool QExiv2::isImgCommentWritable() const
{
	return d->isMetadataWritable(Exiv2::mdComment);
}

