#pragma once
#include <QJsonObject>
#include <QtQmlIntegration>
#include "qepr.h"

class PrivacyPolicy : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, version)
    QM_PROPERTY(QString, message)

    public:
        PrivacyPolicy(QObject* parent = nullptr) : QObject(parent) {};
        PrivacyPolicy(QObject* parent, QJsonObject data) : QObject(parent)
        {
            m_version = data["version"].toString();
            m_message = data["message"].toString();
        };
};
