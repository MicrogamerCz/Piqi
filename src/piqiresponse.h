#pragma once
#include "piqi_export.h"
#include <QNetworkReply>
#include <qjsondocument.h>
#include <qjsonobject.h>

class PIQI_EXPORT PiqiResponse : public QObject {
    Q_OBJECT

    Q_PROPERTY(QVariant data READ data)
    Q_PROPERTY(QString response READ response)
    Q_PROPERTY(int statusCode READ statusCode)
    Q_PROPERTY(bool isSuccessful READ isSuccessful)

  public:
    PiqiResponse(QObject *obj, const QNetworkReply &reply);

    template<typename T>
    static PiqiResponse *buildResponse(QNetworkReply &reply) {
        if (!reply.isFinished())
            return nullptr;

        T *obj = nullptr;
        if (reply.error() == QNetworkReply::NoError) {
            QJsonObject json = QJsonDocument::fromJson(reply.readAll()).object();
            obj = new T(nullptr, json);
        }

        return new PiqiResponse(obj, reply);
    }

    QVariant data();
    QString response();
    int statusCode();
    bool isSuccessful();

  private:
    const QNetworkReply &reply;
    QVariant obj;
};
