#include "tag.h"

Tag::Tag(QObject *parent) : QJObject(parent) {
}

Tag::Tag(QObject *parent, QJsonObject data) : QJObject(parent) {
    deserialize(data);
}

BookmarkTag::BookmarkTag(QObject* parent)
    : Tag(parent)
{
}

BookmarkTag::BookmarkTag(QObject *parent, QJsonObject data) : Tag(parent) {
    deserialize(data);
}
