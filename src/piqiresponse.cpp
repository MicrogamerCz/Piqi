#include "piqiresponse.h"
#include <qjsondocument.h>
#include <qnetworkreply.h>
#include <qobject.h>
#include <qvariant.h>

PiqiResponse::PiqiResponse(QObject *obj, QNetworkReply &reply)
    : QObject(nullptr), reply(reply), obj(QVariant::fromValue(obj)) {
}

QVariant PiqiResponse::data() const {
    return obj;
}
QString PiqiResponse::response() const {
    return reply.errorString();
}
QJsonObject PiqiResponse::body() const {
    QJsonDocument doc = QJsonDocument::fromJson(reply.readAll());
    return doc.object();
}
int PiqiResponse::statusCode() const {
    return reply.attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}
bool PiqiResponse::isSuccessful() const {
    return statusCode() == 200;
}
