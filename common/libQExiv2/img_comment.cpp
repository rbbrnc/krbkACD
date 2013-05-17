#include <QDebug>

#include "QExiv2.h"
#include "QExiv2_p.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Img Comment Functions
//////////////////////////////////////////////////////////////////////////////////////////
bool QExiv2::hasComment() const
{
	return !d->imageComment.empty();
}

QByteArray QExiv2::imgComment() const
{
	return QByteArray(d->imageComment.data(), d->imageComment.size());
}

bool QExiv2::setImgComment(const QByteArray& data)
{
	d->imageComment = std::string(data.data(), data.size());
	return true;
}

bool QExiv2::clearImgComment()
{
	return setImgComment(QByteArray());
}

bool QExiv2::isImgCommentWritable() const
{
	return d->isMetadataWritable(Exiv2::mdComment);
}

