#ifndef SOCIAL_METADATA_H
#define SOCIAL_METADATA_H

#include <QString>
#include <QList>

class SocialMetadata
{
	public:
		SocialMetadata();
		~SocialMetadata();

		struct Comment {
			QString date;
			QString author;
			QString authorUri;
			QString text;
		};

		QString albumTitle() const;
		void setAlbumTitle(const QString &title);

		QString albumDescription() const;
		void setAlbumDescription(const QString &desc);

		QString publisher() const;
		void setPublisher(const QString &name);

		QString publisherUri() const;
		void setPublisherUri(const QString &uri);

		QString publisherComment() const;
		void setPublisherComment(const QString &text);

		QString datetime() const;
		void setDatetime(const QString &dt);

		QList<struct SocialMetadata::Comment> comments() const;
		void setComments(QList<struct SocialMetadata::Comment> comments);
		void addComment(const struct SocialMetadata::Comment &comment);

	private:
		QString m_albumTitle;
		QString m_albumDescription;
		QString m_publisherName;
		QString m_publisherUri;
		QString m_publisherComment;
		QString m_datetimePublished;
		QList<struct SocialMetadata::Comment> m_comments;
};

#endif
