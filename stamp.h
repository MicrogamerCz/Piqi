#pragma once
#include <QtQmlIntegration>
#include <QJsonObject>
#include "qepr.h"

class Stamp : public QObject {
    Q_OBJECT
    QML_ELEMENT;

    QM_PROPERTY(int, id)
    QM_PROPERTY(QString, url)

    public:
        Stamp(QObject* parent = nullptr) : QObject(parent) {};
        Stamp(QObject* parent, QJsonObject data) : QObject(parent)
        {
            m_id = data["stamp_id"].toInt();
            m_url = data["stamp_url"].toString();
        }
};
