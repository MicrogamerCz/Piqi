// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Micro <microgamercz@proton.me>

#include "notifications.h"
#include "notification.h"
#include "requestworker.h"
#include <qvariant.h>

Notifications::Notifications(QObject *parent) : QAbstractListModel(parent) {
}
Notifications::Notifications(QObject *parent, QJsonObject data) : QAbstractListModel(parent) {
    beginResetModel();
    for (QJsonValue il : data["Notifications"].toArray())
        m_notifications.append(new Notification(this, il.toObject()));
    endResetModel();

    m_nextUrl = data.keys().contains("next_url") ? data["next_url"].toString() : m_nextUrl = "";
}
QCoro::QmlTask Notifications::nextNotifications() {
    return nextNotificationsTask();
}
QCoro::Task<> Notifications::nextNotificationsTask() {
    Notifications *feed = co_await PiqiInternal::sendGet<Notifications>(QUrl(m_nextUrl));
    extend(feed);
}

int Notifications::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_notifications.count();
}
QVariant Notifications::data(const QModelIndex &index, int role) const {
    const Notification *illustration = m_notifications[index.row()];
    return (role == CustomRoles::IllustRole) ? QVariant::fromValue(illustration) : QVariant();
}
QHash<int, QByteArray> Notifications::roleNames() const {
    return {
        {CustomRoles::IllustRole, QByteArrayLiteral("illust")},
    };
}
