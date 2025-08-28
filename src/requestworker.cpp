#include "requestworker.h"
#include "user.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qurlquery.h>

QCoro::Task<bool> PiqiInternal::IsLoggedIn()
{
    bool emptyAT = accessToken.isEmpty();
    bool expired = (expiration.toMSecsSinceEpoch() - QDateTime::currentDateTime().toMSecsSinceEpoch()) < 0;
    bool result = !(emptyAT || expired);
    // if (!result)
    // result |= (co_await LoginTask(refreshToken));
    co_return result;
}

QCoro::Task<PiqiResponse*> PiqiInternal::LoginTask(QString refreshToken)
{
    QNetworkRequest request(QUrl("https://oauth.secure.pixiv.net/auth/token"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery obj{{"client_id", "MOBrBDS8blbauoSck0ZfDbtuzpyT"},
                  {"client_secret", "lsACyCD94FhDUtGTXi3QzcFE2uU1hqtDaKeqrdwj"},
                  {"grant_type", "refresh_token"},
                  {"refresh_token", refreshToken}};

    QNetworkReply *reply = co_await manager.post(request, obj.toString().toUtf8());
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200)
        co_return new PiqiResponse(nullptr, reply);

    QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
    accessToken = data["access_token"].toString();
    refreshToken = data["refresh_token"].toString();
    expiration = QDateTime::currentDateTime().addSecs(3600);
    co_return new PiqiResponse(new Account(nullptr, data["user"].toObject()), reply);
}
