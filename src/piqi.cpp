#include "piqi.h"
#include "comments.h"
#include "illustration.h"
#include "illusts.h"
#include "novels.h"
#include "piqiresponse.h"
#include "requestworker.h"
#include "tags.h"
#include "user.h"
#include "userdetails.h"
#include <QUrlQuery>
#include <qcorotask.h>
#include <qdatetime.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qurl.h>
#include <qurlquery.h>

Piqi::Piqi(QObject *parent) : QObject(parent), m_user(nullptr) {
}

QCoro::QmlTask Piqi::walkthrough() {
    return walkthroughTask();
}
QCoro::Task<PiqiResponse *> Piqi::walkthroughTask() {
    return sendGet<Illusts>(QUrl("https://app-api.pixiv.net/v1/walkthrough/illusts"), false);
}

QCoro::QmlTask Piqi::recommendedFeed(QString type, bool includeRanking, bool includePrivacyPolicy) {
    return recommendedFeedTask(type, includeRanking, includePrivacyPolicy);
}
QCoro::Task<PiqiResponse *> Piqi::recommendedFeedTask(QString type, bool includeRanking, bool includePrivacyPolicy) {
    QUrl url("https://app-api.pixiv.net/v1/" % type % "/recommended");
    QUrlQuery query{{"include_ranking_illusts", includeRanking ? "true" : "false"},
                    {"include_privacy_policy", includePrivacyPolicy ? "true" : "false"}};
    url.setQuery(query);

    if (type == "novel")
        return sendGet<RecommendedNovels>(url);
    else
        return sendGet<Recommended>(url);
}

QCoro::QmlTask Piqi::followingFeed(QString type, QString restriction) {
    return followingFeedTask(type, restriction);
}
QCoro::Task<PiqiResponse *> Piqi::followingFeedTask(QString type, QString restriction) {
    // illust (w/manga) - v2/illust
    // novel (w/manga) - v1/novel
    bool isNovel = type == "novel";
    QString urlType = (isNovel ? "v1/novel" : "v2/illust");
    QUrl url("https://app-api.pixiv.net/" % urlType % "/follow?restrict=" % restriction);
    if (isNovel)
        return sendGet<Novels>(url);
    return sendGet<Illusts>(url);
}

QCoro::QmlTask Piqi::userIllusts(User *user, QString type) {
    return userIllustsTask(user, type);
}
QCoro::Task<PiqiResponse *> Piqi::userIllustsTask(User *user, QString type) {
    // In intercepted requests from the official pixiv App, this endpoint returned <6 illusts,
    // usually the showcase ones when profile is opened, or the three suggested ones under an illust.
    // For some reason though, the returned number of illustrations is... more. One run returned 3,
    // other 17, but it's usually ~30 illusts, which doesn't reflect on the official client...
    // For anyone who cares, I do the HTTP intercepting locally, using Http Toolkit (with Frida) and
    // Waydroid instance. really convenient.
    bool isNovel = type == "novels";
    QString urlType = isNovel ? "novels" : "illusts";
    QUrl url("https://app-api.pixiv.net/v1/user/" + urlType);

    QUrlQuery query{{"user_id", QString::number(user->m_id)}};
    if (!isNovel)
        query.addQueryItem("type", type);
    url.setQuery(query);

    if (isNovel)
        return sendGet<Novels>(url);
    return sendGet<Illusts>(url);
}

QCoro::QmlTask Piqi::latestGlobal(QString type) {
    return latestGlobalTask(type);
}
QCoro::Task<PiqiResponse *> Piqi::latestGlobalTask(QString type) {
    QUrl url("https://app-api.pixiv.net/v1/" % type % "/new");
    QUrlQuery query{{"filter", "for_android"}}; // ? is it necessary

    bool isNovel = type == "novel";
    if (!isNovel)
        query.addQueryItem("content_type", type);
    url.setQuery(query);

    if (isNovel)
        return sendGet<Novels>(url);
    return sendGet<Illusts>(url);
}

QCoro::QmlTask Piqi::bookmarksFeed(User *user, bool restricted, QString tag, QString type) {
    return bookmarksFeedTask(user, restricted, tag, type);
}
QCoro::Task<PiqiResponse *> Piqi::bookmarksFeedTask(User *user, bool restricted, QString tag, QString type) {
    if (!user)
        user = m_user;

    QUrl url("https://app-api.pixiv.net/v1/user/bookmarks/" + type);
    QUrlQuery query{{"user_id", QString::number(user->m_id)}, {"restrict", restricted ? "private" : "public"}};
    if (tag != "")
        query.addQueryItem("tag", tag);
    url.setQuery(query);

    if (type == "novel")
        return sendGet<Novels>(url);
    return sendGet<Illusts>(url);
}

QCoro::QmlTask Piqi::relatedIllusts(Illustration *illust) {
    return relatedIllustsTask(illust);
}
QCoro::Task<PiqiResponse *> Piqi::relatedIllustsTask(Illustration *illust) {
    QUrl url("https://app-api.pixiv.net/v2/illust/related");
    QUrlQuery query{{"illust_id", QString::number(illust->m_id)}};
    url.setQuery(query);
    return sendGet<Illusts>(url);
}

QCoro::QmlTask Piqi::seriesFeed(int id) {
    return seriesFeedTask(id);
}
QCoro::Task<PiqiResponse *> Piqi::seriesFeedTask(int id) {
    QUrl url("https://app-api.pixiv.net/v1/illust/series");
    QUrlQuery query{{"illust_series_id", QString::number(id)}};
    url.setQuery(query);
    return sendGet<Series>(url);
}

QCoro::QmlTask Piqi::userSeries(User *user) {
    return userSeriesTask(user);
}
QCoro::Task<PiqiResponse *> Piqi::userSeriesTask(User *user) {
    QUrl url("https://app-api.pixiv.net/v1/user/illust-series");
    QUrlQuery query{{"user_id", QString::number(user->m_id)}};
    url.setQuery(query);
    return sendGet<SeriesDetails>(url);
}

QCoro::QmlTask Piqi::watchlistFeed() {
    return watchlistFeedTask();
}
QCoro::Task<PiqiResponse *> Piqi::watchlistFeedTask() {
    return sendGet<SeriesDetails>(QUrl("https://app-api.pixiv.net/v1/watchlist/manga"));
}

QCoro::QmlTask Piqi::fetchComments(Illustration *illustration) {
    return fetchCommentsTask(illustration);
}
QCoro::Task<PiqiResponse *> Piqi::fetchCommentsTask(Illustration *illustration) {
    QUrl url("https://app-api.pixiv.net/v3/illust/comments");
    QUrlQuery query{{"illust_id", QString::number(illustration->m_id)}};
    url.setQuery(query);
    return sendGet<Comments>(url);
}

QCoro::QmlTask Piqi::commentReplies(Comment *comment) {
    return commentRepliesTask(comment);
}
QCoro::Task<PiqiResponse *> Piqi::commentRepliesTask(Comment *comment) {
    QUrl url("https://app-api.pixiv.net/v2/illust/comment/replies");
    QUrlQuery query{{"comment_id", QString::number(comment->m_id)}};
    url.setQuery(query);
    return sendGet<Comments>(url);
}

QCoro::QmlTask Piqi::details(User *user) {
    return detailsTask(user);
}
QCoro::Task<PiqiResponse *> Piqi::detailsTask(User *user) {
    QUrl url("https://app-api.pixiv.net/v2/user/detail");
    QUrlQuery query{{"user_id", QString::number(user->m_id)}};
    url.setQuery(query);

    return sendGet<UserDetails>(url);
}

QCoro::QmlTask Piqi::bookmarkTags(QString type, bool restricted) {
    return bookmarkTagsTask(type, restricted);
}
QCoro::Task<PiqiResponse *> Piqi::bookmarkTagsTask(QString type, bool restricted) {
    QUrl url("https://app-api.pixiv.net/v1/user/bookmark-tags/" + type);
    QUrlQuery query{{"user_id", QString::number(m_user->m_id)}, {"restrict", restricted ? "private" : "public"}};
    url.setQuery(query);

    return sendGet<Tags>(url);
}

QCoro::QmlTask Piqi::illustDetail(int id) {
    return illustDetailTask(id);
}
QCoro::Task<PiqiResponse *> Piqi::illustDetailTask(int id) {
    QUrl url("https://app-api.pixiv.net/v1/illust/detail");
    QUrlQuery query{{"illust_id", QString::number(id)}};
    url.setQuery(query);

    return sendGet<Illustration>(url);
}

QCoro::QmlTask Piqi::illustSeriesDetails(Illustration *illust) {
    return illustSeriesDetailsTask(illust);
}
QCoro::Task<PiqiResponse *> Piqi::illustSeriesDetailsTask(Illustration *illust) {
    QUrl url("https://app-api.pixiv.net/v1/illust-series/illust");
    QUrlQuery query{{"illust_id", QString::number(illust->m_id)}};
    url.setQuery(query);

    return sendGet<IllustSeries>(url);
}

QCoro::QmlTask Piqi::login(QString refreshToken) {
    return loginTask(refreshToken);
}
QCoro::Task<PiqiResponse *> Piqi::loginTask(QString refreshToken) {
    PiqiResponse *response = co_await PiqiInternal::LoginTask(refreshToken);
    m_user = response->data().value<Account *>();
    Q_EMIT userChanged();

    co_return response;
}

QCoro::QmlTask Piqi::searchAutocomplete(QString query) {
    return searchAutocompleteTask(query);
}
QCoro::Task<QList<Tag *>> Piqi::searchAutocompleteTask(QString query) {
    QUrl url("https://app-api.pixiv.net/v2/search/autocomplete");
    QUrlQuery params{{"merge_plain_keyword_results", "true"}, {"word", query}};
    url.setQuery(params);

    QNetworkRequest request = co_await createRequest(url);
    QNetworkReply *reply = co_await manager.get(request);
    QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();

    QList<Tag *> tags;
    for (QJsonValue val : obj["tags"].toArray())
        tags.append(new Tag(nullptr, val.toObject()));

    co_return tags;
}

QCoro::QmlTask Piqi::fetchNovel(Novel *novel) {
    return fetchNovelTask(novel);
}
QCoro::Task<QString> Piqi::fetchNovelTask(Novel *novel) {
    QUrl url("https://app-api.pixiv.net/webview/v2/novel");
    QUrlQuery query{
        // TODO: optional parameters, parameters other than id are based on the default settings
        {"id", QString::number(novel->m_id)},
        {"font", "default"},
        {"font_size", "16.0px"},
        {"line_height", "1.75"},
        {"color", "#B7B7B7"},
        {"background_color", "#1F1F1F"},
        {"margin_top", "56px"},
        {"margin_bottom", "53px"},
        {"theme", "dark"},
        {"use_bloc", "true"},
        {"viewer_version", "20250616_seasonal_effect"},
        {"restricted_mode", "false"},
    };
    url.setQuery(query);

    QNetworkRequest request = co_await createRequest(url);
    request.setRawHeader(
        "X-Requested-With",
        "jp.pxv.android"); // NECESSARY! Without this header the endpoint returns (probably random) binary data

    QNetworkReply *reply = co_await manager.get(request);
    co_return reply->readAll();
}

void Piqi::setLogin(QString accessToken, QString refreshToken) {
    PiqiInternal::accessToken = accessToken;
    PiqiInternal::refreshToken = refreshToken;
    PiqiInternal::expiration = QDateTime::currentDateTime().addSecs(3600);
}

QCoro::Task<QNetworkRequest> Piqi::createRequest(const QUrl &url, bool authenticated) const {
    QNetworkRequest request(url);
    if (authenticated) {
        if (QDateTime::currentDateTime().msecsTo(PiqiInternal::expiration) < 0)
            co_await PiqiInternal::LoginTask(PiqiInternal::refreshToken);

        request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    }

    request.setRawHeader("Connection", "Keep-Alive");
    request.setHeader(QNetworkRequest::UserAgentHeader, "PixivAndroidApp/6.183.0 (Android 13; WayDroid x86_64 Device)");
    request.setRawHeader("Host", "app-api.pixiv.net");

    co_return request;
}
template<typename T>
QCoro::Task<PiqiResponse *> Piqi::sendGet(const QUrl &url, bool authenticated) {
    QNetworkRequest request = co_await createRequest(url, authenticated);
    co_return co_await sendGet<T>(request);
}
template<typename T>
QCoro::Task<PiqiResponse *> Piqi::sendGet(const QNetworkRequest &request) {
    QNetworkReply *reply = co_await manager.get(request);
    co_return PiqiResponse::buildResponse<T>(*reply);
}
