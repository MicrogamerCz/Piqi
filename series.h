#pragma once
#include <QtQmlIntegration>
#include <QJsonObject>
#include "qepr.h"

class Series : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(int, id)
    QM_PROPERTY(QString, title)

    public:
        Series(QObject* parent = nullptr) : QObject(parent) {};
        Series(QObject* parent, QJsonObject data) : QObject(parent)
        {
            m_id = data["id"].toInt();
            m_title = data["title"].toString();
        };
};
