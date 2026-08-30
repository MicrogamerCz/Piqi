#include "user.h"
#include "requestworker.h"

User::User(QObject *parent) : QJObject(parent), m_profileImageUrls(nullptr) {
}

User::User(QObject *parent, QJsonObject data) : QJObject(parent), m_profileImageUrls(nullptr) {
    deserialize(data);
    // m_isFollowed = data["is_followed"].toBool(); // 0 - not followed, 1 -
    // publicly followed, 2 - privately followed // ?
}
Q_SLOT QCoro::QmlTask User::follow(bool privateFollow) {
    return followTask(privateFollow);
}
QCoro::Task<> User::followTask(bool privateFollow) {
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v1/user/follow/add"));
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery query{{"user_id", QString::number(m_id)}, {"restrict", (privateFollow ? "private" : "public")}};

    m_isFollowed = (privateFollow ? 2 : 1);
    Q_EMIT isFollowedChanged();

    co_await PiqiInternal::manager.post(request, query.toString().toUtf8());
}
Q_SLOT QCoro::QmlTask User::removeFollow() {
    return removeFollowTask();
}
QCoro::Task<> User::removeFollowTask() {
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
QCoro::QmlTask User::followDetail() {
    return followDetailTask();
}
QCoro::Task<FollowDetails *> User::followDetailTask() {
    QUrl url("https://app-api.pixiv.net/v1/user/follow/detail");
    QUrlQuery query{{"user_id", QString::number(m_id)}};
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());

    QNetworkReply *reply = co_await PiqiInternal::manager.get(request);
    QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
    co_return new FollowDetails(nullptr, data["follow_detail"].toObject());
}
void User::assignProperty(const QString &propertyName, const QJsonValue &data) {
    if (propertyName == "profileImageUrls") {
        if (!data.isNull())
            m_profileImageUrls = new ImageUrls(this, data.toObject());
        Q_EMIT profileImageUrlsChanged();
    } else
        QJObject::assignProperty(propertyName, data);
}

Account::Account(QObject *parent) : User(parent) {
}
Account::Account(QObject *parent, QJsonObject data) : User(parent) {
    deserialize(data);
}
