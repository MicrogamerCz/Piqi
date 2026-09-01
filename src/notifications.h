// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Micro <microgamercz@proton.me>

#pragma once
#include "notification.h"
#include "piqi_export.h"
#include "qepr.h"
#include <QAbstractListModel>
#include <qcoroqmltask.h>

class PIQI_EXPORT Notifications : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QList<Notification *>, notifications)
    QM_PROPERTY(QString, nextUrl)

  public:
    Notifications(QObject *parent = nullptr);
    Notifications(QObject *parent, QJsonObject data);

    Q_SLOT QCoro::QmlTask nextNotifications();
    QCoro::Task<> nextNotificationsTask();

    Q_SLOT void extend(Notifications *nextNotifications) {
        m_nextUrl = nextNotifications->m_nextUrl;
        Q_EMIT nextUrlChanged();

        beginInsertRows({}, m_notifications.count(), m_notifications.count() + nextNotifications->m_notifications.count() - 1);
        m_notifications.append(nextNotifications->m_notifications);
        endInsertRows();

        Q_EMIT notificationsChanged();
    }

    enum CustomRoles {
        IllustRole = Qt::UserRole,
    };
    Q_ENUM(CustomRoles)

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};
