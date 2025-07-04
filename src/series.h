#pragma once
#include <QtQmlIntegration>
#include <QJsonObject>
#include "qepr.h"
#include "piqi_export.h"

class PIQI_EXPORT Series : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(int, id)
    QM_PROPERTY(QString, title)

public:
    Series(QObject* parent = nullptr);
    Series(QObject* parent, QJsonObject data);
};
