#include "requestworker.h"
#include "user.h"

QCoro::Task<bool> PiqiInternal::isLoggedIn() {
    bool emptyAT = accessToken.isEmpty();
    bool expired = (expiration.toMSecsSinceEpoch() - QDateTime::currentDateTime().toMSecsSinceEpoch()) < 0;
    bool result = !(emptyAT || expired);
    // if (!result)
    // result |= (co_await LoginTask(refreshToken));
    co_return result;
}

QCoro::Task<PiqiResponse *> PiqiInternal::loginTask(QString refreshToken) {
    QNetworkRequest request(QUrl("https://oauth.secure.pixiv.net/auth/token"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, "PixivAndroidApp/6.183.0 (Android 13; WayDroid x86_64 Device)");
    QUrlQuery obj{{"client_id", "MOBrBDS8blbauoSck0ZfDbtuzpyT"},
                  {"client_secret", "lsACyCD94FhDUtGTXi3QzcFE2uU1hqtDaKeqrdwj"},
                  {"grant_type", "refresh_token"},
                  {"refresh_token", refreshToken}};

    QNetworkReply *reply = co_await manager.post(request, obj.toString().toUtf8());
    const QByteArray content = reply->readAll();
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200)
        co_return new PiqiResponse(nullptr, content, *reply);

    const QJsonObject data = QJsonDocument::fromJson(content).object();
    accessToken = data["access_token"].toString();
    refreshToken = data["refresh_token"].toString();
    expiration = QDateTime::currentDateTime().addSecs(3600);
    co_return new PiqiResponse(new Account(nullptr, data["user"].toObject()), content, *reply);
}
