#pragma once
#include "qepr.h"
#include "qjobject.h"

class PIQI_EXPORT PrivacyPolicy : public QJObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, version)
    QM_PROPERTY(QString, message)

  public:
    PrivacyPolicy(QObject *parent = nullptr);
    PrivacyPolicy(QObject *parent, QJsonObject data);
};
