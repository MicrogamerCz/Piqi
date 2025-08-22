#include "tags.h"
#include "tag.h"
#include <qabstractitemmodel.h>
#include <qjsonobject.h>
#include <qobject.h>
#include <qtpreprocessorsupport.h>

Tags::Tags(QObject* parent) : QAbstractListModel(parent) {}
Tags::Tags(QObject* parent, QJsonObject data, QString accessToken, QString refreshToken) : QAbstractListModel(parent) {
    Q_UNUSED(accessToken);
    Q_UNUSED(refreshToken);

    beginResetModel();
    for (QJsonValue il : data["bookmark_tags"].toArray()) {
        BookmarkTag* tag = new BookmarkTag(nullptr, il.toObject());
        m_tags.append(tag);
    }
    endResetModel();
    if (data.keys().contains("next_url")) m_nextUrl = data["next_url"].toString();
    else m_nextUrl = "";
}
void Tags::Extend(Tags* nextTags) {
    m_nextUrl = nextTags->m_nextUrl;
    Q_EMIT nextUrlChanged();

    beginInsertRows({}, m_tags.count(), m_tags.count() + nextTags->m_tags.count() - 1);
    m_tags.append(nextTags->m_tags);
    endInsertRows();

    Q_EMIT tagsChanged();
}
int Tags::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_tags.count();
}
QVariant Tags::data(const QModelIndex &index, int role) const
{
    const Tag* tag = m_tags[index.row()];
    switch (role) {
        case NameRole: return tag->m_name;
        default: return QVariant::fromValue(tag);
    }
}
QHash<int, QByteArray> Tags::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}
