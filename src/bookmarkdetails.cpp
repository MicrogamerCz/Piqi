#include "bookmarkdetails.h"
#include "qjobject.h"
#include <qtpreprocessorsupport.h>

BookmarkDetails::BookmarkDetails(QObject *parent) : QJObject(parent) {
}
BookmarkDetails::BookmarkDetails(QObject *parent, QJsonObject data, QString accessToken, QString refreshToken)
    : QJObject(parent) {
    Q_UNUSED(accessToken);
    Q_UNUSED(refreshToken);
    deserialize(data);
}

void BookmarkDetails::assignProperty(const QString &propertyName, const QJsonValue &data) {
    if (propertyName == "tags") {
        for (const QJsonValue &com : data.toArray())
            m_tags.append(new BookmarkTag(nullptr, com.toObject()));

        Q_EMIT tagsChanged();
    } else
        QJObject::assignProperty(propertyName, data);
}

FollowDetails::FollowDetails(QObject *parent) : QJObject(parent) {
}
FollowDetails::FollowDetails(QObject *parent, QJsonObject data) : QJObject(parent) {
    deserialize(data);
}
