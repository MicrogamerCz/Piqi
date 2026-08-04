// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Micro <microgamercz@proton.me>

#include "qjobject.h"

const QRegularExpression QJObject::regex("([[:upper:]])");

QJObject::QJObject(QObject *parent) : QObject(parent) {}
QJObject::QJObject(const QJsonObject &data, QObject *parent) : QObject(parent) {
  const QMetaObject *mo = metaObject();

  for (int i = 0; i < mo->propertyCount(); i++) {
    QMetaProperty prop = mo->property(i);
    QVariant value = prop.read(this);

    QObject *qobj = qvariant_cast<QObject *>(value);
    if (qobj)
      continue;

    QString propertyName = QString::fromStdString(prop.name());
    QString jsonPropertyName = propertyName.replace(regex, "_\\L\\1");

    QJsonValue jsonValue = data[jsonPropertyName];
    assignProperty(propertyName, jsonValue);
  }
}

void QJObject::assignProperty(const QString &propertyName, const QJsonValue &data) {
    setProperty(propertyName.toStdString().c_str(), data.toVariant());
}
