#include "work.h"
#include "bookmarkdetails.h"
#include "requestworker.h"
#include <qjsonobject.h>
#include <qnetworkaccessmanager.h>
#include <QNetworkReply>
#include <qnetworkrequest.h>
#include <QCoro/QCoroCore>

WorkPrimitive::WorkPrimitive(QObject* parent)
    : QObject(parent)
{
}

WorkPrimitive::WorkPrimitive(QObject* parent, QJsonObject data)
    : QObject(parent)
{
    m_id = data["id"].toInt();
    m_title = data["title"].toString();
}

Work::Work(QObject* parent) : QObject(parent) { }
Work::Work(QObject* parent, QJsonObject data) : QObject(parent) {
    m_id = data["id"].toInt();
    m_title = data["title"].toString();
    m_imageUrls = new ImageUrls(nullptr, data["image_urls"].toObject());
    m_caption = data["caption"].toString();
    m_restricted = data["restrict"].toInt();
    m_user = new User(nullptr, data["user"].toObject());
    for (QJsonValue tag : data["tags"].toArray())
        m_tags.append(new Tag(nullptr, tag.toObject()));
    m_createDate = QDateTime::fromString(data["create_date"].toString(), Qt::ISODateWithMs);
    Q_EMIT createDateChanged();
    m_pageCount = data["page_count"].toInt();
    m_xRestrict = data["x_restrict"].toInt();
    m_totalView = data["total_view"].toInt();
    m_totalBookmarks = data["total_bookmarks"].toInt();
    m_isBookmarked = data["is_bookmarked"].toBool();
    m_visible = data["visible"].toBool();
    m_isMuted = data["is_muted"].toBool();
}

QCoro::QmlTask Work::AddBookmark(bool isPrivate) { return AddBookmarkTask(isPrivate); }
QCoro::Task<void> Work::AddBookmarkTask(bool isPrivate)
{
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
    QNetworkReply* reply = manager.post(request, query.toString().toUtf8());

    co_await qCoro(&manager, &QNetworkAccessManager::finished);

    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) co_return;

    m_isBookmarked = bookmarkState;
    Q_EMIT isBookmarkedChanged();
    m_totalBookmarks = totalBookmarksCache;
    Q_EMIT totalBookmarksChanged();
}
QCoro::QmlTask Work::RemoveBookmark() { return RemoveBookmarkTask(); }
QCoro::Task<void> Work::RemoveBookmarkTask()
{
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

    QNetworkReply* reply = manager.post(request, query.toString().toUtf8());

    co_await qCoro(&manager, &QNetworkAccessManager::finished);

    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) co_return;

    m_isBookmarked = bookmarkState;
    Q_EMIT isBookmarkedChanged();
    m_totalBookmarks = totalBookmarksCache;
    Q_EMIT totalBookmarksChanged();
}
QCoro::QmlTask Work::BookmarkDetail() { return BookmarkDetailTask(); }
QCoro::Task<BookmarkDetails*> Work::BookmarkDetailTask() {
    QNetworkAccessManager manager;
    QUrl url("https://app-api.pixiv.net/v2/" + type() + "/bookmark/detail");
    QUrlQuery query{{"illust_id", QString::number(m_id)}};
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    QNetworkReply* reply = manager.get(request);

    co_await qCoro(&manager, &QNetworkAccessManager::finished);

    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    co_return new BookmarkDetails(nullptr, json);
}
