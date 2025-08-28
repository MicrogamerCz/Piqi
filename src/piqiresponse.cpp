#include "piqiresponse.h"
#include <qnetworkreply.h>
#include <qobject.h>

PiqiResponse::PiqiResponse(QObject* parent) : QObject(parent) {}
PiqiResponse::PiqiResponse(QObject* obj, QNetworkReply* reply)
    : QObject(nullptr), reply(reply), obj(obj) {}

QObject* PiqiResponse::getData() {
    return obj;
}
QString PiqiResponse::getResponse() {
    return reply->errorString();
}
int PiqiResponse::getStatusCode() {
    return reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}
bool PiqiResponse::getIsSuccessful() {
    return getStatusCode() == 200;
}
