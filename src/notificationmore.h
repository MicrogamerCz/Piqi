// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Micro <microgamercz@proton.me>

#pragma once
#include "qjobject.h"
#include "workspace.h"
#include <qjsonobject.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class PIQI_EXPORT NotificationMore : public QJObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(bool, unreadExists)
    QM_PROPERTY(QString, title)

  public:
    NotificationMore(QObject *parent = nullptr);
    NotificationMore(QObject *parent, QJsonObject data);
};
