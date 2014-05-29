#include "QExiv2.h"
#include "qexiv2_p.h"
#include "md_social.h"

SocialMetadata QExiv2::socialMetadata()
{
	if (!xmpRegisterNamespace("social/", "social")) {
		return SocialMetadata();
	}

	SocialMetadata smd;

	smd.setAlbumTitle(xmpTagString("Xmp.social.AlbumTitle", true));
	smd.setAlbumDescription(xmpTagString("Xmp.social.AlbumDescription", true));
	smd.setPublisher(xmpTagString("Xmp.social.PublisherName", true));
	smd.setPublisherUri(xmpTagString("Xmp.social.PublisherUri", true));
	smd.setDatetime(xmpTagString("Xmp.social.DateTimePublished", true));

	// Xmp.social.PublisherComment can be mapped on dc:description
	smd.setPublisherComment(xmpTagString("Xmp.social.PublisherComment"));

	for (int i = 1; ; i++) {
		if (xmpTagString(QString("Xmp.social.Comments[%1]").arg(i)).isEmpty()) {
			break;
		}
		struct SocialMetadata::Comment cmn;
		cmn.date      = xmpTagString(QString("Xmp.social.Comments[%1]/social:CommentDate").arg(i));
		cmn.author    = xmpTagString(QString("Xmp.social.Comments[%1]/social:CommentAuthor").arg(i));
		cmn.authorUri = xmpTagString(QString("Xmp.social.Comments[%1]/social:CommentAuthorUri").arg(i));
		cmn.text      = xmpTagString(QString("Xmp.social.Comments[%1]/social:CommentText").arg(i));

		smd.addComment(cmn);
	}

	return smd;
}

bool QExiv2::setSocialMetadata(const SocialMetadata &smd, bool writeComments)
{
	if (!xmpRegisterNamespace("social/", "social")) {
		qWarning() << __PRETTY_FUNCTION__ << "Metadata *NOT* saved";
		return false;
	}

	setXmpTagString("Xmp.social.AlbumTitle", smd.albumTitle());
	setXmpTagString("Xmp.social.AlbumDescription", smd.albumDescription());
	setXmpTagString("Xmp.social.PublisherName", smd.publisher());
	setXmpTagString("Xmp.social.PublisherUri", smd.publisherUri());
	setXmpTagString("Xmp.social.PublisherComment", smd.publisherComment());
	setXmpTagString("Xmp.social.DateTimePublished", smd.datetime());

	if (!writeComments) {
		return true;
	}

	// Update Comments
	removeXmpBag("Xmp.social.Comments", 19);

	QList<struct SocialMetadata::Comment> clist = smd.comments();

	if (clist.count() > 0) {
		setXmpTagBag("Xmp.social.Comments");
		for (int i = 0; i < clist.count(); i++) {
			setXmpTagString(QString("Xmp.social.Comments[%1]/social:CommentDate").arg(i+1), clist.at(i).date);
			setXmpTagString(QString("Xmp.social.Comments[%1]/social:CommentAuthor").arg(i+1),  clist.at(i).author);
			setXmpTagString(QString("Xmp.social.Comments[%1]/social:CommentAuthorUri").arg(i+1), clist.at(i).authorUri);
			setXmpTagString(QString("Xmp.social.Comments[%1]/social:CommentText").arg(i+1), clist.at(i).text);
		}
	}
	return true;
}

