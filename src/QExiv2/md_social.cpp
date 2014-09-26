#include "md_social.h"

#include <QJsonArray>

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

QJsonObject SocialMetadata::toJson() const
{
    QJsonObject obj;

    QJsonObject album;
    QJsonObject publisher;
    QJsonObject comments;
    QJsonArray  array;

    if (!m_albumTitle.isEmpty()) {
        album.insert("title", QJsonValue(m_albumTitle));
    }
    if (!m_albumDescription.isEmpty()) {
        album.insert("caption", QJsonValue(m_albumDescription));
    }


    if (!m_publisherName.isEmpty()) {
        publisher.insert("name", QJsonValue(m_publisherName));
    }

    if (!m_publisherComment.isEmpty()) {
        publisher.insert("caption", QJsonValue(m_publisherComment));
    }

    if (!m_publisherUri.isEmpty()) {
        publisher.insert("uri", QJsonValue(m_publisherUri));
    }

    comments.insert("count", QJsonValue(m_comments.count()));

    for (int i = 0; i < m_comments.count(); i++) {
		QJsonObject comment;
        comment.insert("date", QJsonValue(m_comments.at(i).date));
        comment.insert("author", QJsonValue(m_comments.at(i).author));
        comment.insert("author_uri", QJsonValue(m_comments.at(i).authorUri));
        comment.insert("text", QJsonValue(m_comments.at(i).text));
        array.append(QJsonValue(comment));
    }

    comments.insert("data", QJsonValue(array));

    obj.insert("publisher", QJsonValue(publisher));
    obj.insert("album",     QJsonValue(album));
    obj.insert("comments",  QJsonValue(comments));

    return obj;
}
