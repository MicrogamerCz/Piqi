#pragma once
#include "user.h"
#include "piqiresponse.h"
#include <QNetworkAccessManager>
#include <QDateTime>
#include <QCoro>
#include <qcorotask.h>

namespace PiqiInternal {
    inline QNetworkAccessManager manager;
    inline QString accessToken, refreshToken;
    inline QDateTime expiration;

    QCoro::Task<PiqiResponse*> LoginTask(QString refreshToken = "");
    QCoro::Task<bool> IsLoggedIn();

    template<class T> QCoro::Task<T*> SendGet(QUrl url, bool authenticated = true) {
        if (authenticated && !(co_await IsLoggedIn()))
            co_return nullptr;

        QNetworkRequest request(url);
        if (authenticated) request.setRawHeader("Authorization", ("Bearer " + accessToken).toUtf8());
        QNetworkReply *reply = co_await manager.get(request);
        QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
        co_return new T(nullptr, json);
    }
};
