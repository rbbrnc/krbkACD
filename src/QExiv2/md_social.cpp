#include "md_social.h"

SocialMetadata::SocialMetadata()
{
}

SocialMetadata::~SocialMetadata()
{
}

QString SocialMetadata::albumTitle() const
{
	return m_albumTitle;
}

void SocialMetadata::setAlbumTitle(const QString &title)
{
	m_albumTitle = title;
}

QString SocialMetadata::albumDescription() const
{
	return m_albumDescription;
}

void SocialMetadata::setAlbumDescription(const QString &desc)
{
	m_albumDescription = desc;
}

QString SocialMetadata::publisher() const
{
	return m_publisherName;
}

void SocialMetadata::setPublisher(const QString &name)
{
	m_publisherName = name;
}

QString SocialMetadata::publisherUri() const
{
	return m_publisherUri;
}

void SocialMetadata::setPublisherUri(const QString &uri)
{
	m_publisherUri = uri;
}

QString SocialMetadata::publisherComment() const
{
	return m_publisherComment;
}

void SocialMetadata::setPublisherComment(const QString &text)
{
	m_publisherComment = text;
}

QString SocialMetadata::datetime() const
{
	return m_datetimePublished;
}

void SocialMetadata::setDatetime(const QString &dt)
{
	m_datetimePublished = dt;
}

QList<struct SocialMetadata::Comment> SocialMetadata::comments() const
{
	return m_comments;
}

void SocialMetadata::setComments(QList<struct SocialMetadata::Comment> comments)
{
	m_comments = comments;
}

void SocialMetadata::addComment(const struct SocialMetadata::Comment &comment)
{
	m_comments.append(comment);
}

