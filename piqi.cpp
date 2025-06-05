#include "piqi.h"
#include "comments.h"
#include "illustration.h"
#include "illusts.h"
#include "searchrequest.h"
#include "tag.h"
#include "userdetails.h"
#include <QJsonDocument>
#include <QUrlQuery>
#include <qcborvalue.h>
#include <qcontainerfwd.h>
#include <qcoroqmltask.h>
#include <qcorotask.h>
#include <qdatetime.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qlist.h>
#include <qlogging.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qstringview.h>
#include <qtmetamacros.h>
#include <qtypes.h>
#include <qurl.h>
#include <qurlquery.h>

Piqi::Piqi(QObject *parent)
    : QObject(parent)
{
    m_user = nullptr;
    Q_EMIT userChanged();
}

QCoro::Task<bool> Piqi::IsLoggedIn()
{
    bool emptyAT = accessToken.isEmpty();
    bool expired = (expiration.toMSecsSinceEpoch() - QDateTime::currentDateTime().toMSecsSinceEpoch()) < 0;
    bool result = !(emptyAT || expired);
    if (!result)
        result |= (co_await LoginTask(refreshToken));
    co_return result;
}

template<class T> QCoro::Task<T*> Piqi::SendGet(QUrl url, bool authenticated) {
    if (authenticated && !(co_await IsLoggedIn()))
        co_return nullptr;

    QNetworkRequest request(url);
    if (authenticated) request.setRawHeader("Authorization", ("Bearer " + accessToken).toUtf8());
    QNetworkReply *reply = co_await manager.get(request);
    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    co_return new T(nullptr, json);
}

void Piqi::SetLogin(QString accessToken, QString refreshToken)
{
    this->accessToken = accessToken;
    this->refreshToken = refreshToken;
    expiration = QDateTime::currentDateTime().addSecs(3600);
}

QCoro::QmlTask Piqi::Login(QString refreshToken) { return LoginTask(refreshToken); }
QCoro::Task<bool> Piqi::LoginTask(QString refreshToken)
{
    QNetworkRequest request(QUrl("https://oauth.secure.pixiv.net/auth/token"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery obj{{"client_id", "MOBrBDS8blbauoSck0ZfDbtuzpyT"},
                  {"client_secret", "lsACyCD94FhDUtGTXi3QzcFE2uU1hqtDaKeqrdwj"},
                  {"grant_type", "refresh_token"},
                  {"refresh_token", refreshToken}};
    QNetworkReply *reply = co_await manager.post(request, obj.toString().toUtf8());
    QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
    accessToken = data["access_token"].toString();
    refreshToken = data["refresh_token"].toString();
    expiration = QDateTime::currentDateTime().addSecs(3600);
    m_user = new Account(nullptr, data["user"].toObject());
    Q_EMIT userChanged();
    co_return (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200);
}

QCoro::QmlTask Piqi::Walkthrough() { return WalkthroughTask(); }
QCoro::Task<Illusts *> Piqi::WalkthroughTask() {
    co_return (co_await SendGet<Illusts>(QUrl("https://app-api.pixiv.net/v1/walkthrough/illusts"), false));
}

QCoro::QmlTask Piqi::FetchNextFeed(Illusts* feed) {
    return FetchNextFeedTask(feed);
}
QCoro::Task<Illusts*> Piqi::FetchNextFeedTask(Illusts* feed) {
    co_return (co_await SendGet<Illusts>(QUrl(feed->m_nextUrl)));
}

QCoro::QmlTask Piqi::RecommendedFeed(QString type, bool includeRanking, bool includePrivacyPolicy) {
    return RecommendedFeedTask(type, includeRanking, includePrivacyPolicy);
}
QCoro::Task<Recommended *> Piqi::RecommendedFeedTask(QString type, bool includeRanking, bool includePrivacyPolicy)
{
    QUrl url("https://app-api.pixiv.net/v1/" + type + "/recommended");
    QUrlQuery query{
        {"include_ranking_illusts", includeRanking ? "true" : "false"},
        {"include_privacy_policy", includePrivacyPolicy ? "true" : "false"}
    };
    if (type != "novel")
        url.setQuery(query);
    co_return (co_await SendGet<Recommended>(url));
}

QCoro::QmlTask Piqi::FollowingFeed(QString type, QString restriction) { return FollowingFeedTask(type, restriction); }
QCoro::Task<Illusts *> Piqi::FollowingFeedTask(QString type, QString restriction)
{
    QUrl url(("https://app-api.pixiv.net/v2/" + type + "/follow?restrict=" + restriction));
    co_return (co_await SendGet<Illusts>(url));
}

QCoro::QmlTask Piqi::AddBookmark(Illustration *illust, bool isPrivate) { return AddBookmarkTask(illust, isPrivate); }
QCoro::Task<void> Piqi::AddBookmarkTask(Illustration *illust, bool isPrivate)
{
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v2/illust/bookmark/add"));
    request.setRawHeader("Authorization", ("Bearer " + accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery query{{"illust_id", QString::number(illust->m_id)}, {"restrict", (isPrivate ? "private" : "public")}};
    if (illust->m_isBookmarked == 0) {
        illust->m_totalBookmarks++;
        Q_EMIT illust->totalBookmarksChanged();
    }
    illust->m_isBookmarked = (isPrivate ? 2 : 1);
    Q_EMIT illust->isBookmarkedChanged();
    co_await manager.post(request, query.toString().toUtf8());
}
QCoro::QmlTask Piqi::RemoveBookmark(Illustration *illust)
{
    return RemoveBookmarkTask(illust);
}
QCoro::Task<void> Piqi::RemoveBookmarkTask(Illustration *illust)
{
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v1/illust/bookmark/delete"));
    request.setRawHeader("Authorization", ("Bearer " + accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery query{
        {"illust_id", QString::number(illust->m_id)},
    };
    illust->m_isBookmarked = 0;
    Q_EMIT illust->isBookmarkedChanged();
    illust->m_totalBookmarks--;
    Q_EMIT illust->totalBookmarksChanged();
    co_await manager.post(request, query.toString().toUtf8());
}

QCoro::QmlTask Piqi::UserIllusts(User* user, QString type) { return UserIllustsTask(user, type); }
QCoro::Task<QList<Illustration *>> Piqi::UserIllustsTask(User* user, QString type)
{
    // In intercepted requests from the official pixiv App, this endpoint returned <6 illusts,
    // usually the showcase ones when profile is opened, or the three suggested ones under an illust.
    // For some reason though, the returned number of illustrations is... more. One run returned 3,
    // other 17, but it's usually ~30 illusts, which doesn't reflect on the official client...
    // For anyone who cares, I do the HTTP intercepting locally, using Http Toolkit (with Frida) and
    // Waydroid instance. really convenient.
    QUrl url("https://app-api.pixiv.net/v1/user/illusts");
    QUrlQuery query{
        {"user_id", QString::number(user->m_id)},
        {"type", type}
    };
    url.setQuery(query);

    Illusts* illusts = co_await SendGet<Illusts>(url);
    co_return illusts->m_illusts;
}

QCoro::QmlTask Piqi::IllustComments(Illustration *illust) { return IllustCommentsTask(illust); }
QCoro::Task<Comments*> Piqi::IllustCommentsTask(Illustration *illust)
{
    QUrl url("https://app-api.pixiv.net/v3/illust/comments");
    QUrlQuery query{{"illust_id", QString::number(illust->m_id)}};
    url.setQuery(query);
    co_return (co_await SendGet<Comments>(url));
}
QCoro::QmlTask Piqi::CommentReplies(Comment* comment) { return CommentRepliesTask(comment); }
QCoro::Task<Comments*> Piqi::CommentRepliesTask(Comment* comment) {
    QUrl url("https://app-api.pixiv.net/v2/illust/comment/replies");
    QUrlQuery query{{"comment_id", QString::number(comment->m_id)}};
    url.setQuery(query);
    co_return (co_await SendGet<Comments>(url));
}

QCoro::QmlTask Piqi::BookmarkDetail(Illustration *illust) { return BookmarkDetailTask(illust); }
QCoro::Task<BookmarkDetails *> Piqi::BookmarkDetailTask(Illustration *illust)
{
    QUrl url("https://app-api.pixiv.net/v2/illust/bookmark/detail");
    QUrlQuery query{{"illust_id", QString::number(illust->m_id)}};
    url.setQuery(query);
    co_return (co_await SendGet<BookmarkDetails>(url));
}

QCoro::QmlTask Piqi::Follow(User *user, bool privateFollow) { return FollowTask(user, privateFollow); }
QCoro::Task<void> Piqi::FollowTask(User *user, bool privateFollow)
{
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v1/user/follow/add"));
    request.setRawHeader("Authorization", ("Bearer " + accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery query{{"user_id", QString::number(user->m_id)}, {"restrict", (privateFollow ? "private" : "public")}};
    user->m_isFollowed = (privateFollow ? 2 : 1);
    Q_EMIT user->isFollowedChanged();
    co_await manager.post(request, query.toString().toUtf8());
}
QCoro::QmlTask Piqi::RemoveFollow(User *user) { return RemoveFollowTask(user); }
QCoro::Task<void> Piqi::RemoveFollowTask(User *user)
{
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v1/user/follow/delete"));
    request.setRawHeader("Authorization", ("Bearer " + accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery query{
        {"user_id", QString::number(user->m_id)},
    };
    user->m_isFollowed = 0;
    Q_EMIT user->isFollowedChanged();
    co_await manager.post(request, query.toString().toUtf8());
}

QCoro::QmlTask Piqi::FollowDetail(User *user) { return FollowDetailTask(user); }
QCoro::Task<FollowDetails *> Piqi::FollowDetailTask(User *user)
{
    QUrl url("https://app-api.pixiv.net/v1/user/follow/detail");
    QUrlQuery query{{"user_id", QString::number(user->m_id)}};
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + accessToken).toUtf8());

    QNetworkReply *reply = co_await manager.get(request);
    QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
    co_return new FollowDetails(nullptr, data["follow_detail"].toObject());
}

QCoro::QmlTask Piqi::RelatedIllusts(Illustration *illust) { return RelatedIllustsTask(illust); }
QCoro::Task<Illusts *> Piqi::RelatedIllustsTask(Illustration *illust)
{
    QUrl url("https://app-api.pixiv.net/v2/illust/related");
    QUrlQuery query{{"illust_id", QString::number(illust->m_id)}};
    url.setQuery(query);
    co_return (co_await SendGet<Illusts>(url));
}

QCoro::QmlTask Piqi::SearchAutocomplete(QString query) { return SearchAutocompleteTask(query); }
QCoro::Task<QList<Tag*>> Piqi::SearchAutocompleteTask(QString query) {
    QUrl url("https://app-api.pixiv.net/v2/search/autocomplete");
    QUrlQuery params {
        { "merge_plain_keyword_results", "true" },
        { "word", query }
    };
    url.setQuery(params);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + accessToken).toUtf8());

    QNetworkReply* reply = co_await manager.get(request);
    QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();

    QList<Tag*> tags;
    for (QJsonValue val : obj["tags"].toArray()) {
        Tag* tag = new Tag(nullptr, val.toObject());
        tags.append(tag);
    }

    co_return tags;
}

QCoro::QmlTask Piqi::SearchPopularPreview(SearchRequest* params) {
    return SearchPopularPreviewTask(params);
}
QCoro::Task<Illusts*> Piqi::SearchPopularPreviewTask(SearchRequest* params) {
    QUrl url("https://app-api.pixiv.net/v1/search/popular-preview/illust");
    QUrlQuery query;

    QString words;
    for (int i = 0; i < params->m_tags.length(); i++) {
        Tag* tag = params->m_tags[i];
        words += tag->m_name + " ";
    }
    words.removeLast();
    query.addQueryItem("word", words);

    switch (params->m_searchTarget) {
        case SearchRequest::SearchTarget::PartialTagsMatch: {
            query.addQueryItem("search_target", "partial_match_for_tags");
            break;
        }
        case SearchRequest::SearchTarget::ExactTagsMatch: {
            query.addQueryItem("search_target", "exact_match_for_tags");
            break;
        }
        case SearchRequest::SearchTarget::TitleAndDescription: {
            query.addQueryItem("search_target", "title_and_caption");
            break;
        }
    }

    if (params->m_end_date != nullptr) {
        if (params->m_start_date)
            query.addQueryItem("start_date", params->m_start_date->toString(Qt::DateFormat::ISODate));
        else
            query.addQueryItem("start_date", QDate::currentDate().toString(Qt::DateFormat::ISODate));

        query.addQueryItem("end_date", params->m_end_date->toString(Qt::DateFormat::ISODate));
    }

    url.setQuery(query);
    co_return (co_await SendGet<Illusts>(url));
}

QCoro::QmlTask Piqi::Search(SearchRequest* params) {
    return SearchTask(params);
}
QCoro::Task<SearchResults*> Piqi::SearchTask(SearchRequest* params) {
    QUrl url("https://app-api.pixiv.net/v1/search/illust");
    QUrlQuery query;

    QString words;
    for (int i = 0; i < params->m_tags.length(); i++) {
        Tag* tag = params->m_tags[i];
        words += tag->m_name + " ";
    }
    words.removeLast();
    query.addQueryItem("word", words);

    switch (params->m_searchTarget) {
        case SearchRequest::SearchTarget::PartialTagsMatch: {
            query.addQueryItem("search_target", "partial_match_for_tags");
            break;
        }
        case SearchRequest::SearchTarget::ExactTagsMatch: {
            query.addQueryItem("search_target", "exact_match_for_tags");
            break;
        }
        case SearchRequest::SearchTarget::TitleAndDescription: {
            query.addQueryItem("search_target", "title_and_caption");
            break;
        }
    }

    if (params->m_end_date != nullptr) {
        if (params->m_start_date)
            query.addQueryItem("start_date", params->m_start_date->toString(Qt::DateFormat::ISODate));
        else
            query.addQueryItem("start_date", QDate::currentDate().toString(Qt::DateFormat::ISODate));

        query.addQueryItem("end_date", params->m_end_date->toString(Qt::DateFormat::ISODate));
    }

    if (params->m_sortAscending)
        query.addQueryItem("sort", "date_asc");
    else
        query.addQueryItem("sort", "date_desc");

    url.setQuery(query);

    co_return (co_await SendGet<SearchResults>(url));
}

QCoro::QmlTask Piqi::LatestGlobal(QString type) {
    return LatestGlobalTask(type);
}
QCoro::Task<Illusts*> Piqi::LatestGlobalTask(QString type) {
    if (type == "novel") co_return nullptr;
    co_return (co_await SendGet<Illusts>(QUrl("https://app-api.pixiv.net/v1/illust/new?filter=for_android&content_type=" + type)));
}

QCoro::QmlTask Piqi::BookmarksFeed(QString type, QString restriction) {
    return BookmarksFeedTask(type, restriction);
}
QCoro::Task<Illusts*> Piqi::BookmarksFeedTask(QString type, QString restriction) {
    if (type == "novel") co_return nullptr;
    QUrl url("https://app-api.pixiv.net/v1/user/bookmarks/" + type);
    QUrlQuery query {
        { "user_id", QString::number(m_user->m_id) },
        { "restrict", restriction }
    };
    url.setQuery(query);

    co_return (co_await SendGet<Illusts>(url));
}

QCoro::QmlTask Piqi::Details(User* user) {
    return DetailsTask(user);
}
QCoro::Task<UserDetails*> Piqi::DetailsTask(User* user) {
    QUrl url("https://app-api.pixiv.net/v2/user/detail");
    QUrlQuery query {
        { "user_id", QString::number(user->m_id) }
    };
    url.setQuery(query);

    co_return (co_await SendGet<UserDetails>(url));
}
