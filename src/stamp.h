#pragma once
#include <QtQmlIntegration>
#include <QJsonObject>
#include "qepr.h"
#include "piqi_export.h"

class PIQI_EXPORT Stamp : public QObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(int, id)
    QM_PROPERTY(QString, url)

    public:
        Stamp(QObject* parent = nullptr);
        Stamp(QObject* parent, QJsonObject data);
};
