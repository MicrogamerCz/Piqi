#pragma once
#include "comment.h"

class PIQI_EXPORT Comments : public QJObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QList<Comment *>, comments)
    QM_PROPERTY(QString, nextUrl)
    QM_PROPERTY(int, commentAccessControl)

  public:
    Comments(QObject *parent = nullptr);
    Comments(QObject *parent, QJsonObject data, QString accessToken = "", QString refreshToken = "");

  private:
    void assignProperty(const QString &propertyName, const QJsonValue &data) override;
};
