#include "bookmarkdetails.h"
#include <qtpreprocessorsupport.h>

BookmarkDetails::BookmarkDetails(QObject *parent) : QObject(parent) { }
BookmarkDetails::BookmarkDetails(QObject *parent, QJsonObject data, QString accessToken, QString refreshToken) : QObject(parent)
{
    Q_UNUSED(accessToken);
    Q_UNUSED(refreshToken);

    m_isBookmarked = data["is_bookmarked"].toBool();
    for (QJsonValue com : data["tags"].toArray()) {
        m_tags.append(new BookmarkTag(nullptr, com.toObject()));
    }
    m_restriction = data["restrict"].toString();
    Q_EMIT restrictionChanged();
}

FollowDetails::FollowDetails(QObject *parent) : QObject(parent) { }
FollowDetails::FollowDetails(QObject *parent, QJsonObject data) : QObject(parent)
{
    m_isFollowed = data["is_followed"].toBool();
    m_restriction = data["restrict"].toString();
    Q_EMIT restrictionChanged();
}
