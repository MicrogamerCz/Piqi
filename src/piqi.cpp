#include "piqi.h"
#include "bookmarkdetails.h"
#include "comments.h"
#include "illustration.h"
#include "illusts.h"
#include "novel.h"
#include "novels.h"
#include "searchrequest.h"
#include "series.h"
#include "tag.h"
#include "userdetails.h"
#include "requestworker.h"
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
#include <qtpreprocessorsupport.h>
#include <qtypes.h>
#include <qurl.h>
#include <qurlquery.h>

Piqi::Piqi(QObject *parent)
    : QObject(parent)
{
    m_user = nullptr;
    Q_EMIT userChanged();
}

void Piqi::SetLogin(QString accessToken, QString refreshToken)
{
    PiqiInternal::accessToken = accessToken;
    PiqiInternal::refreshToken = refreshToken;
    PiqiInternal::expiration = QDateTime::currentDateTime().addSecs(3600);
}

QCoro::QmlTask Piqi::Login(QString refreshToken) { return LoginTask(refreshToken); }
QCoro::Task<bool> Piqi::LoginTask(QString refreshToken) {
    m_user = co_await PiqiInternal::LoginTask(refreshToken);
    co_return (m_user != nullptr);
}

QCoro::QmlTask Piqi::Walkthrough() { return WalkthroughTask(); }
QCoro::Task<Illusts *> Piqi::WalkthroughTask() {
    return PiqiInternal::SendGet<Illusts>(QUrl("https://app-api.pixiv.net/v1/walkthrough/illusts"), false);

}

QCoro::QmlTask Piqi::RecommendedFeed(QString type, bool includeRanking, bool includePrivacyPolicy) {
    return RecommendedFeedTask(type, includeRanking, includePrivacyPolicy);
}
QCoro::Task<Recommended *> Piqi::RecommendedFeedTask(QString type, bool includeRanking, bool includePrivacyPolicy)
{
    if (type == "novel") co_return nullptr;

    QUrl url("https://app-api.pixiv.net/v1/" + type + "/recommended");
    QUrlQuery query{
        {"include_ranking_illusts", includeRanking ? "true" : "false"},
        {"include_privacy_policy", includePrivacyPolicy ? "true" : "false"}
    };
    url.setQuery(query);
    co_return (co_await PiqiInternal::SendGet<Recommended>(url));
}
QCoro::QmlTask Piqi::RecommendedNovelsFeed(bool includeRanking, bool includePrivacyPolicy) {
    return RecommendedNovelsFeedTask(includeRanking, includePrivacyPolicy);
}
QCoro::Task<RecommendedNovels*> Piqi::RecommendedNovelsFeedTask(bool includeRanking, bool includePrivacyPolicy) {
    QUrl url("https://app-api.pixiv.net/v1/novel/recommended");
    QUrlQuery query{
        {"include_ranking_novels", includeRanking ? "true" : "false"},
        {"include_privacy_policy", includePrivacyPolicy ? "true" : "false"}
    };
    url.setQuery(query);
    return PiqiInternal::SendGet<RecommendedNovels>(url);
}

QCoro::QmlTask Piqi::FollowingFeed(QString restriction) { return FollowingFeedTask(restriction); }
QCoro::Task<Illusts *> Piqi::FollowingFeedTask(QString restriction)
{
    QUrl url(("https://app-api.pixiv.net/v2/illust/follow?restrict=" + restriction));
    return PiqiInternal::SendGet<Illusts>(url);
}


QCoro::QmlTask Piqi::UserIllusts(User* user, QString type) { return UserIllustsTask(user, type); }
QCoro::Task<Illusts*> Piqi::UserIllustsTask(User* user, QString type)
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
    return PiqiInternal::SendGet<Illusts>(url);
}

QCoro::QmlTask Piqi::CommentReplies(Comment* comment) { return CommentRepliesTask(comment); }
QCoro::Task<Comments*> Piqi::CommentRepliesTask(Comment* comment) {
    QUrl url("https://app-api.pixiv.net/v2/illust/comment/replies");
    QUrlQuery query{{"comment_id", QString::number(comment->m_id)}};
    url.setQuery(query);
    return PiqiInternal::SendGet<Comments>(url);
}


QCoro::QmlTask Piqi::Follow(User *user, bool privateFollow) { return FollowTask(user, privateFollow); }
QCoro::Task<void> Piqi::FollowTask(User *user, bool privateFollow)
{
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v1/user/follow/add"));
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
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
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
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
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());

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
    return PiqiInternal::SendGet<Illusts>(url);
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
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());

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
    return PiqiInternal::SendGet<Illusts>(url);
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

    return PiqiInternal::SendGet<SearchResults>(url);
}

QCoro::QmlTask Piqi::LatestGlobal(QString type) {
    return LatestGlobalTask(type);
}
QCoro::Task<Illusts*> Piqi::LatestGlobalTask(QString type) {
    if (type == "novel") co_return nullptr;
    co_return co_await PiqiInternal::SendGet<Illusts>(QUrl("https://app-api.pixiv.net/v1/illust/new?filter=for_android&content_type=" + type));
}

QCoro::QmlTask Piqi::BookmarksFeed(User* user, bool restricted, QString tag) {
    return BookmarksFeedTask(user, restricted, tag);
}
QCoro::Task<Illusts*> Piqi::BookmarksFeedTask(User* user, bool restricted, QString tag) {
    if (!user) user = m_user;
    QUrl url("https://app-api.pixiv.net/v1/user/bookmarks/illust");
    QUrlQuery query {
        { "user_id", QString::number(user->m_id) },
        { "restrict", restricted ? "private" : "public" }
    };
    if (tag != "") query.addQueryItem("tag", tag);
    url.setQuery(query);

    return PiqiInternal::SendGet<Illusts>(url);
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

    return PiqiInternal::SendGet<UserDetails>(url);
}

QCoro::QmlTask Piqi::BookmarkTags(QString type, bool restricted) {
    return BookmarkTagsTask(type, restricted);
}
QCoro::Task<Tags*> Piqi::BookmarkTagsTask(QString type, bool restricted) {
    Q_UNUSED(type)
    QUrl url("https://app-api.pixiv.net/v1/user/bookmark-tags/illust");
    QUrlQuery query {
        { "user_id", QString::number(m_user->m_id) },
        { "restrict", restricted ? "private" : "public" }
    };
    url.setQuery(query);

    return PiqiInternal::SendGet<Tags>(url);
}

QCoro::QmlTask Piqi::IllustDetail(int id) {
    return IllustDetailTask(id);
}
QCoro::Task<Illustration*> Piqi::IllustDetailTask(int id) {
    QUrl url("https://app-api.pixiv.net/v1/illust/detail");
    QUrlQuery query {
        { "illust_id", QString::number(id) }
    };
    url.setQuery(query);

    if (!(co_await PiqiInternal::IsLoggedIn()))
        co_return nullptr;

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    QNetworkReply *reply = co_await manager.get(request);
    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();

    co_return new Illustration(nullptr, json["illust"].toObject());
}

QCoro::QmlTask Piqi::NovelsBookmarksFeed(User* user, bool restricted, QString tag) {
    return NovelsBookmarksFeedTask(user, restricted, tag);
}
QCoro::Task<Novels*> Piqi::NovelsBookmarksFeedTask(User* user, bool restricted, QString tag) {
    if (!user) user = m_user;
    QUrl url("https://app-api.pixiv.net/v1/user/bookmarks/novel");
    QUrlQuery query {
        { "user_id", QString::number(user->m_id) },
        { "restrict", restricted ? "private" : "public" }
    };
    if (tag != "") query.addQueryItem("tag", tag);
    url.setQuery(query);

    return PiqiInternal::SendGet<Novels>(url);
}
QCoro::QmlTask Piqi::FollowingNovelsFeed(QString restriction) {
    return FollowingNovelsFeedTask(restriction);
}
QCoro::Task<Novels*> Piqi::FollowingNovelsFeedTask(QString restriction) {
    QUrl url(("https://app-api.pixiv.net/v1/novel/follow?restrict=" + restriction));
    return PiqiInternal::SendGet<Novels>(url);
}

QCoro::QmlTask Piqi::LatestNovelsGlobal() { return LatestNovelsGlobalTask(); }
QCoro::Task<Novels*> Piqi::LatestNovelsGlobalTask() {
    return PiqiInternal::SendGet<Novels>(QUrl("https://app-api.pixiv.net/v1/novel/new"));
}

QCoro::QmlTask Piqi::SearchNovelsPopularPreview(SearchRequest* params) {
    return SearchNovelsPopularPreviewTask(params);
}
QCoro::Task<Novels*> Piqi::SearchNovelsPopularPreviewTask(SearchRequest* params) {
    QUrl url("https://app-api.pixiv.net/v1/search/popular-preview/novel");
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
    return PiqiInternal::SendGet<Novels>(url);
}

QCoro::QmlTask Piqi::SearchNovels(SearchRequest* params) {
    return SearchNovelsTask(params);
}
QCoro::Task<NovelSearchResults*> Piqi::SearchNovelsTask(SearchRequest* params) {
    QUrl url("https://app-api.pixiv.net/v1/search/novel");
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

    return PiqiInternal::SendGet<NovelSearchResults>(url);
}

QCoro::QmlTask Piqi::UserNovels(User* user) {
    return UserNovelsTask(user);
}
QCoro::Task<Novels*> Piqi::UserNovelsTask(User* user) {
    QUrl url("https://app-api.pixiv.net/v1/user/novels");
    QUrlQuery query{
        {"user_id", QString::number(user->m_id)}
    };
    url.setQuery(query);
    return PiqiInternal::SendGet<Novels>(url);
}

QCoro::QmlTask Piqi::FetchNovel(Novel* novel) { return FetchNovelTask(novel); }
QCoro::Task<QString> Piqi::FetchNovelTask(Novel* novel) {
    if (!(co_await PiqiInternal::IsLoggedIn()))
        co_return nullptr;

    QUrl url("https://app-api.pixiv.net/webview/v2/novel");
    QUrlQuery query { // TODO: optional parameters, parameters other than id are based on the default settings
        {"id",QString::number(novel->m_id)},
        {"font","default"},
        {"font_size","16.0px"},
        {"line_height","1.75"},
        {"color","#B7B7B7"},
        {"background_color","#1F1F1F"},
        {"margin_top","56px"},
        {"margin_bottom","53px"},
        {"theme","dark"},
        {"use_bloc","true"},
        {"viewer_version","20250616_seasonal_effect"},
        {"restricted_mode","false"},
    };
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    request.setRawHeader("X-Requested-With", "jp.pxv.android"); // NECESSARY! Without this header the endpoint returns (probably random) binary data
    QNetworkReply *reply = co_await manager.get(request);
    co_return reply->readAll();
}

QCoro::QmlTask Piqi::IllustSeriesDetails(Illustration* illust) {
    return IllustSeriesDetailsTask(illust);
}
QCoro::Task<IllustSeries*> Piqi::IllustSeriesDetailsTask(Illustration* illust) {
    QUrl url("https://app-api.pixiv.net/v1/illust-series/illust");
    QUrlQuery query{
        {"illust_id", QString::number(illust->m_id)}
    };
    url.setQuery(query);
    return PiqiInternal::SendGet<IllustSeries>(url);
}

QCoro::QmlTask Piqi::SeriesFeed(int id) {
    return SeriesFeedTask(id);
}
QCoro::Task<Series*> Piqi::SeriesFeedTask(int id) {
    QUrl url("https://app-api.pixiv.net/v1/illust/series");
    QUrlQuery query {
        {"illust_series_id", QString::number(id)}
    };
    url.setQuery(query);
    return PiqiInternal::SendGet<Series>(url);
}

QCoro::QmlTask Piqi::UserSeries(User* user) {
    return UserSeriesTask(user);
}
QCoro::Task<SeriesDetails*> Piqi::UserSeriesTask(User* user) {
    QUrl url("https://app-api.pixiv.net/v1/user/illust-series");
    QUrlQuery query{
        {"user_id", QString::number(user->m_id)}
    };
    url.setQuery(query);
    return PiqiInternal::SendGet<SeriesDetails>(url);
}
QCoro::QmlTask Piqi::WatchlistFeed() {
    return WatchlistFeedTask();
}
QCoro::Task<SeriesDetails*> Piqi::WatchlistFeedTask() {
    return PiqiInternal::SendGet<SeriesDetails>(QUrl("https://app-api.pixiv.net/v1/watchlist/manga"));
}

QCoro::QmlTask Piqi::WatchlistAdd(SeriesDetail* series, QString type) {
    return WatchlistAddTask(series, type);
}
QCoro::Task<> Piqi::WatchlistAddTask(SeriesDetail* series, QString type) {
    Q_UNUSED(type);
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v1/watchlist/manga/add"));
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery query{
        {"series_id", QString::number(series->m_id)},
    };
    co_await manager.post(request, query.toString().toUtf8());
}
QCoro::QmlTask Piqi::WatchlistDelete(SeriesDetail* series, QString type) {
    return WatchlistDeleteTask(series, type);
}
QCoro::Task<> Piqi::WatchlistDeleteTask(SeriesDetail* series, QString type) {
    Q_UNUSED(type);
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v1/watchlist/manga/delete"));
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery query{
        {"series_id", QString::number(series->m_id)},
    };
    co_await manager.post(request, query.toString().toUtf8());
}
