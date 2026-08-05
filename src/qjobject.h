// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Micro <microgamercz@proton.me>

#pragma once
#include "piqi_export.h"
#include <QJsonObject>
#include <QRegularExpression>
#include <QtQmlIntegration> // clazy:exclude=no-module-include
#include <qjsonvalue.h>

// TODO: automatic property assignment for descending objects
/*#define QJM_PROPERTY(type, name) \
    QM_PROPERTY(type *, name) \
    Q_INVOKABLE type *create_##name(const QJsonObject &data) { \
        return new type(data, this);\
    }
#define QJM_ARRAY(type, name) \
    QM_PROPERTY(QList<type *>, name) \
    Q_INVOKABLE type *create_##name(const QJsonObject &data) { \
        return new type(data, this);\
    }*/

/*
 * Helper class for deserializing JSON data into QObjects. Constructor with QJsonObject
 * automatically sets property values. This class cannot handle JSON objects and arrays
 * yet (for these types, override 'assignProperty' and call the base method for default
 * case)
 *
 * Property names must follow camelCase, json properties must follow snake_case
 */
class PIQI_EXPORT QJObject : public QObject {
  Q_OBJECT
  QML_ELEMENT

  // ? wanted to use this for something
  // static constexpr std::string_view QJKEY = "QJsonSerializable";
  // Q_CLASSINFO("QJsonSerializable", "true")

public:
  QJObject(QObject *parent = nullptr);
  QJObject(const QJsonObject &data, QObject *parent = nullptr);
  virtual ~QJObject() = default;

protected:
  static const QRegularExpression regex;

  // Use this virtual method to assign custom objects or arrays into properties
  virtual void assignProperty(const QString &propertyName, const QJsonValue &data);
};
