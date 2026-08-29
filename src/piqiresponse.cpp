#include "piqiresponse.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qnetworkreply.h>
#include <qobject.h>
#include <qvariant.h>

PiqiResponse::PiqiResponse(QObject *obj, const QNetworkReply &reply)
    : QObject(nullptr), reply(reply), obj(QVariant::fromValue(obj)) {
}

template<typename T>
PiqiResponse *buildResponse(QNetworkReply &reply) {
    if (!reply.isFinished())
        return nullptr;

    T *obj = nullptr;
    if (reply.error() == QNetworkReply::NoError) {
        QJsonObject json = QJsonDocument::fromJson(reply.readAll()).object();
        obj = new T(nullptr, json);
    }

    return new PiqiResponse(obj, reply);
}

QVariant PiqiResponse::data() {
    return obj;
}
QString PiqiResponse::response() {
    return reply.errorString();
}
int PiqiResponse::statusCode() {
    return reply.attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}
bool PiqiResponse::isSuccessful() {
    return statusCode() == 200;
}
