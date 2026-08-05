// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Micro <microgamercz@proton.me>

#include "qjobject.h"
#include <qjsonvalue.h>

const QRegularExpression QJObject::regex("([[:upper:]])");

QJObject::QJObject(QObject *parent) : QObject(parent) {}
QJObject::QJObject(const QJsonObject &data, QObject *parent) : QObject(parent) {
    // TODO: add and handle required properties
    for (QString key : data.keys()) {
        QString propertyName = key.replace(regex, "\\U\\1");
        assignProperty(propertyName, data[key]);
    }
}

void QJObject::assignProperty(const QString &propertyName, const QJsonValue &data) {
    setProperty(propertyName.toStdString().c_str(), data.toVariant());
}
