#pragma once
#include "illustration.h"
#include "novel.h"
#include "piqi_export.h"
#include "piqiresponse.h"
#include "qepr.h"
#include "user.h"
#include <QCoro>
#include <QCoroQmlTask>
#include <QNetworkAccessManager>
#include <qqmlintegration.h>

class PIQI_EXPORT Piqi : public QObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(Account *, user)
    QM_PROPERTY(QList<Account *>, otherUsers)

  public:
    Piqi(QObject *parent = nullptr);

    // Feed methods
    Q_INVOKABLE QCoro::QmlTask Walkthrough();
    QCoro::Task<PiqiResponse *> WalkthroughTask();

    // TODO: replace qstring type for enum - illust, manga, novel
    Q_INVOKABLE QCoro::QmlTask
    recommendedFeed(QString type, bool includeRanking = false, bool includePrivacyPolicy = false);
    QCoro::Task<PiqiResponse *>
    recommendedFeedTask(QString type, bool includeRanking = false, bool includePrivacyPolicy = false);

    // TODO: replace qstring type for enum - illust (w/ manga), novel
    // TODO: replace qstring restriction for enum - all, public, private
    Q_INVOKABLE QCoro::QmlTask followingFeed(QString type, QString restriction);
    QCoro::Task<PiqiResponse *> followingFeedTask(QString type, QString restriction);

    // TODO: replace qstring type for enum - illust, manga, novel
    Q_INVOKABLE QCoro::QmlTask userIllusts(User *user, QString type);
    QCoro::Task<PiqiResponse *> userIllustsTask(User *user,
                                                QString type); // * Check how many illusts are actually returned

    // TODO: replace qstring type for enum - illust, manga, novel
    Q_INVOKABLE QCoro::QmlTask latestGlobal(QString type);
    QCoro::Task<PiqiResponse *> latestGlobalTask(QString type);

    // TODO: replace qstring type for enum - illust (w/ manga), novel
    // TODO: replace qstring restriction for enum - public, private
    Q_INVOKABLE QCoro::QmlTask bookmarksFeed(User *user = nullptr,
                                             bool restricted = false,
                                             QString tag = "",
                                             QString type = "illust"); // Add back user with nullptr value by default
    QCoro::Task<PiqiResponse *> bookmarksFeedTask(User *user = nullptr,
                                                  bool restricted = false,
                                                  QString tag = "",
                                                  QString type = "illust"); // Illusts

    Q_INVOKABLE QCoro::QmlTask relatedIllusts(Illustration *illust);
    QCoro::Task<PiqiResponse *> relatedIllustsTask(Illustration *illust);

    // Other feed methods
    Q_INVOKABLE QCoro::QmlTask seriesFeed(int id);
    QCoro::Task<PiqiResponse *> seriesFeedTask(int id);

    Q_INVOKABLE QCoro::QmlTask userSeries(User *user);
    QCoro::Task<PiqiResponse *> userSeriesTask(User *user);

    Q_INVOKABLE QCoro::QmlTask watchlistFeed();
    QCoro::Task<PiqiResponse *> watchlistFeedTask();

    // Other methods
    Q_INVOKABLE QCoro::QmlTask commentReplies(Comment *comment);
    QCoro::Task<PiqiResponse *> commentRepliesTask(Comment *comment);

    Q_INVOKABLE QCoro::QmlTask details(User *user);
    QCoro::Task<PiqiResponse *> detailsTask(User *user);

    // TODO: replace qstring type for enum - illust (w/ manga), novel
    // TODO: replace qstring restriction for enum - public, private
    Q_INVOKABLE QCoro::QmlTask bookmarkTags(QString type = "illust", bool restricted = false);
    QCoro::Task<PiqiResponse *> bookmarkTagsTask(QString type = "illust", bool restricted = false);

    Q_INVOKABLE QCoro::QmlTask illustDetail(int id);
    QCoro::Task<PiqiResponse *> illustDetailTask(int id);

    Q_INVOKABLE QCoro::QmlTask illustSeriesDetails(Illustration *illust);
    QCoro::Task<PiqiResponse *> illustSeriesDetailsTask(Illustration *illust);

    Q_INVOKABLE QCoro::QmlTask login(QString refreshToken);
    QCoro::Task<PiqiResponse *> loginTask(QString refreshToken);

    Q_INVOKABLE QCoro::QmlTask searchAutocomplete(QString query);
    QCoro::Task<QList<Tag *>>
    searchAutocompleteTask(QString query); // TODO: make it signal-based for early cancellation, piqiresponse

    Q_INVOKABLE QCoro::QmlTask fetchNovel(Novel *novel); // TODO: optional parameters
    QCoro::Task<QString> fetchNovelTask(Novel *novel);   // TODO: optional parameters, piqiresponse

    Q_INVOKABLE void setLogin(QString accessToken, QString refreshToken);

  private:
    QNetworkAccessManager manager;

    QCoro::Task<QNetworkRequest> createRequest(const QUrl &url, bool authenticated = true) const;
    template<typename T>
    QCoro::Task<PiqiResponse *> sendGet(const QUrl &url, bool authenticated = true);
    template<typename T>
    QCoro::Task<PiqiResponse *> sendGet(const QNetworkRequest &request);
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
