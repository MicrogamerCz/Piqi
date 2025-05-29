#include "tag.h"

Tag::Tag(QObject* parent)
    : QObject(parent)
{
}

Tag::Tag(QObject* parent, QJsonObject data)
    : QObject(parent)
{
    m_name = data["name"].toString();
    m_translatedName = data["translated_name"].toString();
}

BookmarkTag::BookmarkTag(QObject* parent)
    : QObject(parent)
{
}

BookmarkTag::BookmarkTag(QObject* parent, QJsonObject data)
    : QObject(parent)
{
    m_name = data["name"].toString();
    m_isRegistered = data["is_registered"].toBool();
}
