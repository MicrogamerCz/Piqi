#include "tags.h"
#include "tag.h"
#include <qabstractitemmodel.h>
#include <qjsonobject.h>
#include <qobject.h>

Tags::Tags(QObject* parent) : QAbstractListModel(parent) {}
Tags::Tags(QObject* parent, QJsonObject data) : QAbstractListModel(parent) {
    beginResetModel();
    for (QJsonValue il : data["bookmark_tags"].toArray()) {
        BookmarkTag* tag = new BookmarkTag(nullptr, il.toObject());
        tags.append(tag);
    }
    endResetModel();
    if (data.keys().contains("next_url")) m_nextUrl = data["next_url"].toString();
    else m_nextUrl = "";
}
int Tags::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return tags.count();
}

QVariant Tags::data(const QModelIndex &index, int role) const
{
    const Tag* tag = tags[index.row()];
    return QVariant::fromValue(tag);
}
