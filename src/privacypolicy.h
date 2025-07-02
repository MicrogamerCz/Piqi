#pragma once
#include <QJsonObject>
#include <QtQmlIntegration>
#include "piqi_export.h"
#include "qepr.h"

class PIQI_EXPORT PrivacyPolicy : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, version)
    QM_PROPERTY(QString, message)

    public:
        PrivacyPolicy(QObject* parent = nullptr);
        PrivacyPolicy(QObject* parent, QJsonObject data);
};
