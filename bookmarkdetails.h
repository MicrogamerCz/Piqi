#pragma once
#include <QJsonArray>
#include "tag.h"

class BookmarkDetails : public QObject
{
    Q_OBJECT
    QML_ELEMENT;

    QM_PROPERTY(bool, isBookmarked)
    QM_PROPERTY(QList<BookmarkTag *>, tags)
    QM_PROPERTY(QString, restriction)

public:
    BookmarkDetails(QObject *parent = nullptr)
        : QObject(parent) { };
    BookmarkDetails(QObject *parent, QJsonObject data)
        : QObject(parent)
    {
        m_isBookmarked = data["is_bookmarked"].toBool();
        for (QJsonValue com : data["tags"].toArray()) {
            m_tags.append(new BookmarkTag(nullptr, com.toObject()));
        }
        m_restriction = data["restrict"].toString();
        Q_EMIT restrictionChanged();
    }
};
class FollowDetails : public QObject
{
    Q_OBJECT
    QML_ELEMENT;

    QM_PROPERTY(bool, isFollowed)
    QM_PROPERTY(QString, restriction)

public:
    FollowDetails(QObject *parent = nullptr)
        : QObject(parent) { };
    FollowDetails(QObject *parent, QJsonObject data)
        : QObject(parent)
    {
        m_isFollowed = data["is_followed"].toBool();
        m_restriction = data["restrict"].toString();
        Q_EMIT restrictionChanged();
    }
};
