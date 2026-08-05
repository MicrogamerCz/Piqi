#include "tags.h"

Tags::Tags(QObject *parent) : QAbstractListModel(parent) {
}
Tags::Tags(QObject *parent, QJsonObject data) : QAbstractListModel(parent) {
    beginResetModel();
    for (const QJsonValue &il : data["bookmark_tags"].toArray())
        m_tags.append(new BookmarkTag(this, il.toObject()));
    endResetModel();

    m_nextUrl = data["next_url"].toString();
}
void Tags::Extend(Tags *nextTags) {
    m_nextUrl = nextTags->m_nextUrl;
    Q_EMIT nextUrlChanged();

    beginInsertRows({}, m_tags.count(), m_tags.count() + nextTags->m_tags.count() - 1);
    for (Tag *tag : nextTags->m_tags) {
        tag->setParent(this);
        m_tags.append(tag);
    }
    endInsertRows();

    Q_EMIT tagsChanged();
}
int Tags::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_tags.count();
}
QVariant Tags::data(const QModelIndex &index, int role) const {
    const Tag *tag = m_tags[index.row()];

    switch (role) {
    case NameRole:
        return tag->m_name;
    case TranslatedNameRole:
        return tag->m_translatedName;
    default:
        return QVariant::fromValue(tag);
    }
}
QHash<int, QByteArray> Tags::roleNames() const {
    return roles;
}
