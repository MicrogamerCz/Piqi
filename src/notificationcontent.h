// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Micro <microgamercz@proton.me>

#pragma once
#include "qjobject.h"
#include "workspace.h"
#include <qjsonobject.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class PIQI_EXPORT NotificationContent : public QJObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, text)
    QM_PROPERTY(QString, leftIcon)
    QM_PROPERTY(QString, leftImage)
    QM_PROPERTY(QString, rightIcon)
    QM_PROPERTY(QString, rightImage)

  public:
    NotificationContent(QObject *parent = nullptr);
    NotificationContent(QObject *parent, QJsonObject data);
};
