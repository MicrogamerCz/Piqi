#include "tag.h"

Tag::Tag(QObject *parent) : QJObject(parent) {
}
Tag::Tag(QObject *parent, QJsonObject data) : QJObject(parent) {
    deserialize(data);
}
bool Tag::operator==(const Tag &other) const {
    return m_name == other.m_name && m_translatedName == other.m_translatedName;
}

BookmarkTag::BookmarkTag(QObject *parent) : Tag(parent) {
}
BookmarkTag::BookmarkTag(QObject *parent, QJsonObject data) : Tag(parent) {
    deserialize(data);
}
