#include "piqiresponse.h"
#include <qjsondocument.h>
#include <qnetworkreply.h>
#include <qobject.h>
#include <qvariant.h>

PiqiResponse::PiqiResponse(QObject *obj, const QByteArray &content, QNetworkReply &reply)
    : QObject(nullptr), content(content), reply(reply), obj(QVariant::fromValue(obj)) {
}

QVariant PiqiResponse::data() const {
    return obj;
}
QString PiqiResponse::response() const {
    return isSuccessful() ? QString::fromUtf8(content) : reply.errorString();
}
QJsonObject PiqiResponse::body() const {
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    return doc.object();
}
int PiqiResponse::statusCode() const {
    return reply.attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}
bool PiqiResponse::isSuccessful() const {
    return statusCode() == 200;
}
