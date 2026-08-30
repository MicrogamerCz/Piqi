#pragma once
#include "piqiresponse.h"
#include <QCoro>
#include <QJsonObject>

namespace PiqiInternal {
inline QNetworkAccessManager manager;
inline QString accessToken, refreshToken;
inline QDateTime expiration;

QCoro::Task<PiqiResponse *> loginTask(QString refreshToken = "");
QCoro::Task<bool> isLoggedIn();

template<class T>
QCoro::Task<T *> sendGet(QUrl url, bool authenticated = true) {
    if (authenticated && !(co_await isLoggedIn()))
        co_return nullptr;

    QNetworkRequest request(url);
    if (authenticated)
        request.setRawHeader("Authorization", ("Bearer " + accessToken).toUtf8());
    QNetworkReply *reply = co_await manager.get(request);
    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    co_return new T(nullptr, json);
}
}; // namespace PiqiInternal
