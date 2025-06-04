#pragma once
#include <QJsonObject>
#include <QtQmlIntegration>
#include "qepr.h"

class Tag : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, name)
    QM_PROPERTY(QString, translatedName)

    public:
        Tag(QObject* parent = nullptr);
        Tag(QObject* parent, QJsonObject data);
};

class BookmarkTag : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, name)
    QM_PROPERTY(bool, isRegistered)

    public:
        BookmarkTag(QObject* parent = nullptr);
        BookmarkTag(QObject* parent, QJsonObject data);
};
