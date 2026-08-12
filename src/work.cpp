#include "work.h"
#include "requestworker.h"
#include <QCoro/QCoroCore>
#include <QNetworkReply>

WorkPrimitive::WorkPrimitive(QObject *parent) : QJObject(parent) {
}
WorkPrimitive::WorkPrimitive(QObject *parent, QJsonObject data) : QJObject(parent) {
    deserialize(data);
}
void WorkPrimitive::assignProperty(const QString &propertyName, const QJsonValue &data) {
    QJObject::assignProperty(propertyName, data);
}

Work::Work(QObject *parent) : WorkPrimitive(parent) {
}
Work::Work(QObject *parent, QJsonObject data) : WorkPrimitive(parent) {
    deserialize(data);
}
QCoro::QmlTask Work::AddBookmark(bool isPrivate) {
    return AddBookmarkTask(isPrivate);
}
QCoro::Task<> Work::AddBookmarkTask(bool isPrivate) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v2/illust/bookmark/add"));
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery query{{"illust_id", QString::number(m_id)}, {"restrict", (isPrivate ? "private" : "public")}};

    int bookmarkState = m_isBookmarked, totalBookmarksCache = m_totalBookmarks;

    if (m_isBookmarked == 0) {
        m_totalBookmarks++;
        Q_EMIT totalBookmarksChanged();
    }
    m_isBookmarked = (isPrivate ? 2 : 1);
    Q_EMIT isBookmarkedChanged();
    QNetworkReply *reply = manager.post(request, query.toString().toUtf8());

    co_await qCoro(&manager, &QNetworkAccessManager::finished);

    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
        co_return;

    m_isBookmarked = bookmarkState;
    Q_EMIT isBookmarkedChanged();
    m_totalBookmarks = totalBookmarksCache;
    Q_EMIT totalBookmarksChanged();
}
QCoro::QmlTask Work::RemoveBookmark() {
    return RemoveBookmarkTask();
}
QCoro::Task<> Work::RemoveBookmarkTask() {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v1/" + type() + "/bookmark/delete"));
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery query{
        {"illust_id", QString::number(m_id)},
    };

    int bookmarkState = m_isBookmarked, totalBookmarksCache = m_totalBookmarks;

    m_isBookmarked = 0;
    Q_EMIT isBookmarkedChanged();
    m_totalBookmarks--;
    Q_EMIT totalBookmarksChanged();

    QNetworkReply *reply = manager.post(request, query.toString().toUtf8());

    co_await qCoro(&manager, &QNetworkAccessManager::finished);

    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
        co_return;

    m_isBookmarked = bookmarkState;
    Q_EMIT isBookmarkedChanged();
    m_totalBookmarks = totalBookmarksCache;
    Q_EMIT totalBookmarksChanged();
}
QCoro::QmlTask Work::BookmarkDetail() {
    return BookmarkDetailTask();
}
QCoro::Task<BookmarkDetails *> Work::BookmarkDetailTask() {
    QNetworkAccessManager manager;
    QUrl url("https://app-api.pixiv.net/v2/" + type() + "/bookmark/detail");
    QUrlQuery query{{"illust_id", QString::number(m_id)}};
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    QNetworkReply *reply = manager.get(request);

    co_await qCoro(&manager, &QNetworkAccessManager::finished);

    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    co_return new BookmarkDetails(nullptr, json); // * check where it's used
}
void Work::assignProperty(const QString &propertyName, const QJsonValue &data) {
    switch (properties.indexOf(propertyName)) {
    case 0: // imageUrls
        if (!data.isNull())
            m_imageUrls = new ImageUrls(this, data.toObject());
        Q_EMIT imageUrlsChanged();
        break;
    case 1: // user
        if (!data.isNull())
            m_user = new User(this, data.toObject());
        Q_EMIT userChanged();
        break;
    case 2: // tags
        for (QJsonValue tag : data.toArray())
            m_tags.append(new Tag(this, tag.toObject()));
        Q_EMIT tagsChanged();
        break;
    case 3: // createDate
        m_createDate = QDateTime::fromString(data.toString(), Qt::ISODateWithMs);
        Q_EMIT createDateChanged();
        break;
    default: // primitive properties
        QJObject::assignProperty(propertyName, data);
    }
}
