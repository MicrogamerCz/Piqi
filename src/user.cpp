#include "user.h"
#include "qjobject.h"
#include "requestworker.h"
#include <qjsonvalue.h>
#include <qvariant.h>

User::User(QObject *parent) : QJObject(parent) {
}

User::User(QObject *parent, QJsonObject data) : QJObject(parent) {
    deserialize(data);
    // m_isFollowed = data["is_followed"].toBool(); // 0 - not followed, 1 -
    // publicly followed, 2 - privately followed // ?
}

Q_SLOT QCoro::QmlTask User::Follow(bool privateFollow) {
    return FollowTask(privateFollow);
}
QCoro::Task<> User::FollowTask(bool privateFollow) {
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v1/user/follow/add"));
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery query {
        {"user_id", QString::number(m_id)},
        {"restrict", (privateFollow ? "private" : "public")}
    };

    m_isFollowed = (privateFollow ? 2 : 1);
    Q_EMIT isFollowedChanged();

    co_await PiqiInternal::manager.post(request, query.toString().toUtf8());
}

Q_SLOT QCoro::QmlTask User::RemoveFollow() {
    return RemoveFollowTask();
}
QCoro::Task<> User::RemoveFollowTask() {
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v1/user/follow/delete"));
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery query{
        {"user_id", QString::number(m_id)},
    };

    m_isFollowed = 0;
    Q_EMIT isFollowedChanged();

    co_await PiqiInternal::manager.post(request, query.toString().toUtf8());
}

QCoro::QmlTask User::FollowDetail() { return FollowDetailTask(); }
QCoro::Task<FollowDetails *> User::FollowDetailTask()
{
    QUrl url("https://app-api.pixiv.net/v1/user/follow/detail");
    QUrlQuery query{{"user_id", QString::number(m_id)}};
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());

    QNetworkReply *reply = co_await PiqiInternal::manager.get(request);
    QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
    co_return new FollowDetails(nullptr, data["follow_detail"].toObject()); // * check where it's used
}

void User::assignProperty(const QString &propertyName, const QJsonValue &data) {
    if (propertyName == "profileImageUrls") {
        m_profileImageUrls = new ImageUrls(this, data.toObject());
        Q_EMIT profileImageUrlsChanged();
    } else
        QJObject::assignProperty(propertyName, data);
}

Account::Account(QObject* parent) : User(parent) {}
Account::Account(QObject *parent, QJsonObject data) : User(parent) {
    deserialize(data);
}
