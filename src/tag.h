#pragma once
#include <QJsonObject>
#include <QtQmlIntegration>
#include "qepr.h"
#include "piqi_export.h"

class PIQI_EXPORT Tag : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, name)
    QM_PROPERTY(QString, translatedName) // optional
    QM_PROPERTY(bool, addedByUploadedUser) // optional

    public:
        Tag(QObject* parent = nullptr);
        Tag(QObject* parent, QJsonObject data);
};

class PIQI_EXPORT BookmarkTag : public Tag
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(bool, isRegistered) // optional
    QM_PROPERTY(int, count) // optional

    public:
        BookmarkTag(QObject* parent = nullptr);
        BookmarkTag(QObject* parent, QJsonObject data);
};
