// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Micro <microgamercz@proton.me>

#pragma once
#include "notificationcontent.h"
#include "notificationmore.h"
#include "qjobject.h"
#include "workspace.h"
#include <qdatetime.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qobject.h>
#include <qtmetamacros.h>

class PIQI_EXPORT Notification : public QJObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(int, id)
    QM_PROPERTY(QDateTime, createdDatetime)
    QM_PROPERTY(int, type)
    QM_PROPERTY(NotificationContent *, content)
    QM_PROPERTY(NotificationMore *, viewMore)
    QM_PROPERTY(QString, targetUrl)
    QM_PROPERTY(bool, isRead)

  public:
    Notification(QObject *parent = nullptr);
    Notification(QObject *parent, QJsonObject data);

  private:
    const QStringList properties = {"createdDatetime", "content", "viewMore"};
    void assignProperty(const QString &propertyName, const QJsonValue &data) override;
};
