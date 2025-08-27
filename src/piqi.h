#pragma once
#include "comments.h"
#include "illustration.h"
#include "illusts.h"
#include "novel.h"
#include "novels.h"
#include "piqi_export.h"
#include <QCoro>
#include <QCoroQmlTask>
#include <QObject>
#include <QQmlEngine>
#include <qcontainerfwd.h>
#include <qcoroprocess.h>
#include <qcoroqmltask.h>
#include <qdatetime.h>
#include <qnetworkaccessmanager.h>
#include <qstringview.h>
#include <qtmetamacros.h>
#include "series.h"
#include "tag.h"
#include "tags.h"
#include "userdetails.h"

class PIQI_EXPORT Piqi : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(Account *, user)
    QM_PROPERTY(QList<Account*>, otherUsers)

    QNetworkAccessManager manager;

public:
    Piqi(QObject *parent = nullptr);

    // Other methods
    QCoro::Task<bool> LoginTask(QString refreshToken);
    QCoro::Task<Comments*> CommentRepliesTask(Comment* comment);
    QCoro::Task<UserDetails*> DetailsTask(User* user);
    QCoro::Task<Tags*> BookmarkTagsTask(QString type = "illust", bool restricted = false);
    QCoro::Task<Illustration*> IllustDetailTask(int id);
    QCoro::Task<QString> FetchNovelTask(Novel* novel); // TODO: optional parameters
    QCoro::Task<IllustSeries*> IllustSeriesDetailsTask(Illustration* illust);

    // Feed methods
    QCoro::Task<Illusts *> WalkthroughTask();
    QCoro::Task<Recommended *> RecommendedFeedTask(QString type, bool includeRanking = false, bool includePrivacyPolicy = false);
    QCoro::Task<RecommendedNovels *> RecommendedNovelsFeedTask(bool includeRanking = false, bool includePrivacyPolicy = false);
    QCoro::Task<Illusts *> FollowingFeedTask(QString restriction);
    QCoro::Task<Illusts*> UserIllustsTask(User* user, QString type);
    QCoro::Task<Illusts*> RelatedIllustsTask(Illustration* illust);
    QCoro::Task<Illusts*> LatestGlobalTask(QString type);
    QCoro::Task<Illusts*> BookmarksFeedTask(User* user = nullptr, bool restricted = false, QString tag = "");
    QCoro::Task<Novels*> NovelsBookmarksFeedTask(User* user = nullptr, bool restricted = false, QString tag = "");
    QCoro::Task<Novels*> FollowingNovelsFeedTask(QString restriction);
    QCoro::Task<Novels*> LatestNovelsGlobalTask();
    QCoro::Task<Novels*> UserNovelsTask(User* user);
    QCoro::Task<Series*> SeriesFeedTask(int id);
    QCoro::Task<SeriesDetails*> UserSeriesTask(User* user);
    QCoro::Task<SeriesDetails*> WatchlistFeedTask();

    // Search methods
    QCoro::Task<QList<Tag*>> SearchAutocompleteTask(QString query);

public Q_SLOTS:
    void SetLogin(QString accessToken, QString refreshToken);

    QCoro::QmlTask Login(QString refreshToken);
    QCoro::QmlTask CommentReplies(Comment* comment);
    QCoro::QmlTask Details(User* user);
    QCoro::QmlTask BookmarkTags(QString type = "illust", bool restricted = false);
    QCoro::QmlTask IllustDetail(int id);
    QCoro::QmlTask FetchNovel(Novel* novel); // TODO: optional parameters
    QCoro::QmlTask IllustSeriesDetails(Illustration* illust);

    QCoro::QmlTask Walkthrough();
    QCoro::QmlTask RecommendedFeed(QString type, bool includeRanking = false, bool includePrivacyPolicy = false);
    QCoro::QmlTask RecommendedNovelsFeed(bool includeRanking = false, bool includePrivacyPolicy = false);
    QCoro::QmlTask FollowingFeed(QString restriction);
    QCoro::QmlTask UserIllusts(User* user, QString type);
    QCoro::QmlTask RelatedIllusts(Illustration* illust);
    QCoro::QmlTask LatestGlobal(QString type);
    QCoro::QmlTask BookmarksFeed(User* user = nullptr, bool restricted = false, QString tag = ""); // Add back user with nullptr value by default
    QCoro::QmlTask NovelsBookmarksFeed(User* user = nullptr, bool restricted = false, QString tag = ""); // Same as with normal BookmarksFeed
    QCoro::QmlTask FollowingNovelsFeed(QString restriction);
    QCoro::QmlTask LatestNovelsGlobal();
    QCoro::QmlTask UserNovels(User* user);
    QCoro::QmlTask SeriesFeed(int id);
    QCoro::QmlTask UserSeries(User* user);
    QCoro::QmlTask WatchlistFeed();

    QCoro::QmlTask SearchAutocomplete(QString query);
};

/*
 * TODO endpoints:
 * /v1/notification/new-from-following [GET]
 * - Parameters:
 *   - last_seen_illust_id: int
 *   - last_seen_novel_id: int
 *   - last_notified_datetime: datetime (yyyy-MM-ddThh:mm:ssZ)
 * - Returns:
 *   {
 *     "notification": {
 *          "title": string
 *          "body" : string
 *          "target_url" : string
 *          "analytics_type" : string
 *      }
 *     "last_seen_illust_id": int
 *     "last_seen_novel_id": int
 *   }
 */
