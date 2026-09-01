#pragma once
#include "piqi_export.h"
#include <QNetworkReply>
#include <qjsondocument.h>
#include <qjsonobject.h>

class PIQI_EXPORT PiqiResponse : public QObject {
    Q_OBJECT

    Q_PROPERTY(QVariant data READ data)
    Q_PROPERTY(QString response READ response)
    Q_PROPERTY(QJsonObject body READ body)
    Q_PROPERTY(int statusCode READ statusCode)
    Q_PROPERTY(bool isSuccessful READ isSuccessful)

  public:
    PiqiResponse(QObject *obj, const QByteArray &content, QNetworkReply &reply);

    template<typename T>
    static PiqiResponse *buildResponse(QNetworkReply &reply, const QString &key = QString()) {
        if (!reply.isFinished())
            return nullptr;

        const QByteArray content = reply.readAll();

        T *obj = nullptr;
        if (reply.error() == QNetworkReply::NoError) {
            const QJsonObject json = QJsonDocument::fromJson(content).object();
            obj = new T(nullptr, key.isEmpty() ? json : json[key].toObject());
        }

        return new PiqiResponse(obj, content, reply);
    }

    QVariant data() const;
    QString response() const;
    QJsonObject body() const;
    int statusCode() const;
    bool isSuccessful() const;

  private:
    const QByteArray content;
    QNetworkReply &reply;
    QVariant obj;
};
