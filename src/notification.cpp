// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Micro <microgamercz@proton.me>

#include "notification.h"
#include "notificationcontent.h"
#include "notificationmore.h"
#include "qjobject.h"

Notification::Notification(QObject *parent) : QJObject(parent), m_content(nullptr), m_viewMore(nullptr) {
}
Notification::Notification(QObject *parent, QJsonObject data)
    : QJObject(parent), m_content(nullptr), m_viewMore(nullptr) {
    deserialize(data);
}

void Notification::assignProperty(const QString &propertyName, const QJsonValue &data) {
    switch (properties.indexOf(propertyName)) {
    case 0: // createdDatetime
        m_createdDatetime = QDateTime::fromString(data.toString(), Qt::ISODate);
        Q_EMIT createdDatetimeChanged();
        break;
    case 1: // content
        m_content = new NotificationContent(this, data.toObject());
        Q_EMIT contentChanged();
        break;
    case 2: // viewMore
        if (data.isNull())
            break;
        m_viewMore = new NotificationMore(this, data.toObject());
        Q_EMIT viewMoreChanged();
        break;
    default:
        QJObject::assignProperty(propertyName, data);
    }
}
