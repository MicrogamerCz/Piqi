#include "tag.h"

Tag::Tag(QObject* parent)
    : QObject(parent)
{
}

Tag::Tag(QObject* parent, QJsonObject data)
    : QObject(parent)
{
    m_name = data["name"].toString();
    if (data.contains("translated_name"))
        m_translatedName = data["translated_name"].toString();
    if (data.contains("added_by_uploaded_user"))
        m_addedByUploadedUser = data["added_by_uploaded_user"].toBool();
}

BookmarkTag::BookmarkTag(QObject* parent)
    : Tag(parent)
{
}

BookmarkTag::BookmarkTag(QObject* parent, QJsonObject data)
    : Tag(parent, data)
{
    if (data.contains("is_registered"))
        m_isRegistered = data["is_registered"].toBool();
    if (data.contains("count"))
        m_count = data["count"].toInt();
    else m_count = 0;
}
