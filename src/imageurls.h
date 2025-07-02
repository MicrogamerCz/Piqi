#pragma once
#include <QJsonObject>
#include <QtQmlIntegration>
#include "qepr.h"
#include "piqi_export.h"

class PIQI_EXPORT ImageUrls : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, squareMedium)
    QM_PROPERTY(QString, medium)
    QM_PROPERTY(QString, large)
    QM_PROPERTY(QString, original)
    QM_PROPERTY(QString, px16)
    QM_PROPERTY(QString, px50)
    QM_PROPERTY(QString, px170)

public:
    ImageUrls(QObject* parent = nullptr);
    ImageUrls(QObject* parent, QJsonObject data);
    ~ImageUrls() override;
};
