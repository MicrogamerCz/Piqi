#pragma once
#include <QJsonObject>
#include <QtQmlIntegration>
#include "qepr.h"

class Tag : public QObject
{
    Q_OBJECT
    QML_ELEMENT;

    QM_PROPERTY(QString, name)
    QM_PROPERTY(QString, translatedName)

    public:
        Tag(QObject* parent = nullptr) : QObject(parent) {};
        Tag(QObject* parent, QJsonObject data) : QObject(parent)
        {
            m_name = data["name"].toString();
            m_translatedName = data["translated_name"].toString();
        };
};
class BookmarkTag : public QObject
{
    Q_OBJECT
    QML_ELEMENT;

    QM_PROPERTY(QString, name)
    QM_PROPERTY(bool, isRegistered)

    public:
        BookmarkTag(QObject* parent = nullptr) : QObject(parent) {};
        BookmarkTag(QObject* parent, QJsonObject data) : QObject(parent)
        {
            m_name = data["name"].toString();
            m_isRegistered = data["is_registered"].toBool();
        };
};
