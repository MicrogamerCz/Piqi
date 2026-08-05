#pragma once
#include "piqi_export.h"
#include "qepr.h"
#include "qjobject.h"

class PIQI_EXPORT ImageUrls : public QJObject {
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
    ImageUrls(QObject *parent = nullptr);
    ImageUrls(QObject *parent, QJsonObject data);

  private:
    void assignProperty(const QString &propertyName, const QJsonValue &data) override;
};
