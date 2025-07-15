#include "work.h"
#include <qjsonobject.h>
#include <qobject.h>

Work::Work(QObject* parent) : QObject(parent) { }
Work::Work(QObject* parent, QJsonObject data) : QObject(parent) {
    m_id = data["id"].toInt();
    m_title = data["title"].toString();
    m_imageUrls = new ImageUrls(nullptr, data["image_urls"].toObject());
    m_caption = data["caption"].toString();
    m_restricted = data["restrict"].toInt();
    m_user = new User(nullptr, data["user"].toObject());
    for (QJsonValue tag : data["tags"].toArray())
        m_tags.append(new Tag(nullptr, tag.toObject()));
    m_createDate = QDateTime::fromString(data["create_date"].toString(), Qt::ISODateWithMs);
    Q_EMIT createDateChanged();
    m_pageCount = data["page_count"].toInt();
    m_xRestrict = data["x_restrict"].toInt();
    m_totalView = data["total_view"].toInt();
    m_totalBookmarks = data["total_bookmarks"].toInt();
    m_isBookmarked = data["is_bookmarked"].toBool();
    m_visible = data["visible"].toBool();
    m_isMuted = data["is_muted"].toBool();
}
