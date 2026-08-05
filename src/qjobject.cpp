// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Micro <microgamercz@proton.me>

#include "qjobject.h"
#include <qjsonvalue.h>

const QRegularExpression QJObject::regex("_(.)");

QJObject::QJObject(QObject *parent) : QObject(parent) {}

void QJObject::deserialize(const QJsonObject &data) {
    // TODO: add and handle required properties
    for (QString key : data.keys()) {
        QString propertyName = capitalizeSnakeFirstLetters(key).replace(regex, "\\1");
        assignProperty(propertyName, data[key]);
    }
}

void QJObject::assignProperty(const QString &propertyName, const QJsonValue &data) {
    setProperty(propertyName.toStdString().c_str(), data.toVariant());
}

QString QJObject::capitalizeSnakeFirstLetters(QString string) const {
    for (int i = 0; i < string.length(); i++) {
        if (string[i] == "_")
            string[i] = string[++i].toUpper();
    }
    return string;
}
