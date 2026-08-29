#include "piqiresponse.h"
#include <qnetworkreply.h>
#include <qobject.h>
#include <qvariant.h>

PiqiResponse::PiqiResponse(QObject *obj, const QNetworkReply &reply)
    : QObject(nullptr), reply(reply), obj(QVariant::fromValue(obj)) {
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
