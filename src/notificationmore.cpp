// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Micro <microgamercz@proton.me>

#include "notificationmore.h"
#include "qjobject.h"

NotificationMore::NotificationMore(QObject *parent) : QJObject(parent) {
}
NotificationMore::NotificationMore(QObject *parent, QJsonObject data) : QJObject(parent) {
    deserialize(data);
}
