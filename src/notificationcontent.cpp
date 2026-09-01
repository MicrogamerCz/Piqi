// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Micro <microgamercz@proton.me>

#include "notificationcontent.h"
#include "qjobject.h"

NotificationContent::NotificationContent(QObject *parent) : QJObject(parent) {}
NotificationContent::NotificationContent(QObject *parent, QJsonObject data) : QJObject(parent) {
    deserialize(data);
}
