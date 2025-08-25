// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 Joshua Goins <josh@redstrate.com>

#include "illusts.h"
#include "requestworker.h"
#include <qabstractitemmodel.h>
#include <qcoroqmltask.h>
#include <qsharedpointer.h>

Illusts::Illusts(QObject* parent)
    : QAbstractListModel(parent) {}
Illusts::Illusts(QObject* parent, QJsonObject data) : QAbstractListModel(parent)
{
    beginResetModel();
    for (QJsonValue il : data["illusts"].toArray()) {
        Illustration* illust = new Illustration(nullptr, il.toObject());
        m_illusts.append(illust);
    }
    endResetModel();
    if (data.keys().contains("next_url")) m_nextUrl = data["next_url"].toString();
    else m_nextUrl = "";
}

QCoro::QmlTask Illusts::NextFeed() {
    return NextFeedTask();
}
QCoro::Task<> Illusts::NextFeedTask() {
    Illusts* feed = co_await PiqiInternal::SendGet<Illusts>(QUrl(m_nextUrl));
    Extend(feed);
}

int Illusts::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_illusts.count();
}

QVariant Illusts::data(const QModelIndex &index, int role) const
{
    const auto illustration = m_illusts[index.row()];
    if (role == CustomRoles::IllustRole) {
        return QVariant::fromValue(illustration);
    }
    return {};
}

QHash<int, QByteArray> Illusts::roleNames() const
{
    return {
        {CustomRoles::IllustRole, QByteArrayLiteral("illust")},
    };
}
