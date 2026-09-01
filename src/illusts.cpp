// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 Joshua Goins <josh@redstrate.com>

#include "illusts.h"
#include "requestworker.h"

Illusts::Illusts(QObject *parent) : QAbstractListModel(parent) {
}
Illusts::Illusts(QObject *parent, QJsonObject data) : QAbstractListModel(parent) {
    beginResetModel();
    for (QJsonValue il : data["illusts"].toArray())
        m_illusts.append(new Illustration(this, il.toObject()));
    endResetModel();
    if (data.keys().contains("next_url"))
        m_nextUrl = data["next_url"].toString();
    else
        m_nextUrl = "";
}
QCoro::QmlTask Illusts::nextFeed() {
    return nextFeedTask();
}
QCoro::Task<> Illusts::nextFeedTask() {
    Illusts *feed = co_await PiqiInternal::sendGet<Illusts>(QUrl(m_nextUrl));
    extend(feed);
}
int Illusts::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_illusts.count();
}
QVariant Illusts::data(const QModelIndex &index, int role) const {
    const auto illustration = m_illusts[index.row()];
    if (role == CustomRoles::IllustRole) {
        return QVariant::fromValue(illustration);
    }
    return {};
}
QHash<int, QByteArray> Illusts::roleNames() const {
    return {
        {CustomRoles::IllustRole, QByteArrayLiteral("illust")},
    };
}
