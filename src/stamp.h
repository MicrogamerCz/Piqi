#pragma once
#include "qepr.h"
#include "qjobject.h"

class PIQI_EXPORT Stamp : public QJObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(int, stampId)
    QM_PROPERTY(QString, stampUrl)

  public:
    Stamp(QObject *parent = nullptr);
    Stamp(QObject *parent, QJsonObject data);
};
