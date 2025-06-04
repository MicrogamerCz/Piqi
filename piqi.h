#pragma once
#include "bookmarkdetails.h"
#include "comments.h"
#include "illustration.h"
#include "illusts.h"
#include "searchrequest.h"
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

// TODO: merge next feed requests into one

class Piqi : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(Account *, user)
    QM_PROPERTY(QList<Account*>, otherUsers)

    QNetworkAccessManager manager;
    QString accessToken = "", refreshToken = "";
    QDateTime expiration;

    template<class T> QCoro::Task<T*> SendGet(QUrl url, bool authenticated = true);
    QCoro::Task<bool> IsLoggedIn();

public:
    Piqi(QObject *parent = nullptr);
    QCoro::Task<bool> LoginTask(QString refreshToken);
    QCoro::Task<Illusts *> WalkthroughTask();
    QCoro::Task<Recommended *> RecommendedFeedTask(QString type, bool includeRanking = false, bool includePrivacyPolicy = false);
    QCoro::Task<Illusts *> FollowingFeedTask(QString type, QString restriction);
    QCoro::Task<void> AddBookmarkTask(Illustration *illust, bool isPrivate = false);
    QCoro::Task<void> RemoveBookmarkTask(Illustration *illust);
    QCoro::Task<QList<Illustration*>> UserIllustsTask(User* user, QString type);
    QCoro::Task<Comments*> IllustCommentsTask(Illustration* illust);
    QCoro::Task<Comments*> CommentRepliesTask(Comment* comment);
    QCoro::Task<BookmarkDetails*> BookmarkDetailTask(Illustration* illust);
    QCoro::Task<void> FollowTask(User* user, bool privateFollow = false);
    QCoro::Task<void> RemoveFollowTask(User* user);
    QCoro::Task<FollowDetails*> FollowDetailTask(User* user);
    QCoro::Task<Illusts*> RelatedIllustsTask(Illustration* illust);
    QCoro::Task<QList<Tag*>> SearchAutocompleteTask(QString query);
    QCoro::Task<Illusts*> SearchPopularPreviewTask(SearchRequest* params);
    QCoro::Task<SearchResults*> SearchTask(SearchRequest* params);
    QCoro::Task<Illusts*> LatestGlobalTask(QString type);
    QCoro::Task<Illusts*> BookmarksFeedTask(QString type, QString restriction);

    QCoro::Task<Recommended *> RecommendedFeedNextTask(Recommended* recommended);
    QCoro::Task<Illusts *> FollowingFeedNextTask(Illusts* feed);
    QCoro::Task<SearchResults*> SearchNextTask(SearchResults* results);
    QCoro::Task<Illusts*> LatestGlobalTaskNext(Illusts* illusts);
    QCoro::Task<Illusts*> BookmarksFeedTaskNext(Illusts* illusts);

public Q_SLOTS:
    void SetLogin(QString accessToken, QString refreshToken);
    QCoro::QmlTask Login(QString refreshToken);
    QCoro::QmlTask Walkthrough();
    QCoro::QmlTask RecommendedFeed(QString type, bool includeRanking = false, bool includePrivacyPolicy = false);
    QCoro::QmlTask FollowingFeed(QString type, QString restriction);
    QCoro::QmlTask AddBookmark(Illustration *illust, bool isPrivate = false);
    QCoro::QmlTask RemoveBookmark(Illustration *illust);
    QCoro::QmlTask UserIllusts(User* user, QString type);
    QCoro::QmlTask IllustComments(Illustration* illust);
    QCoro::QmlTask CommentReplies(Comment* comment);
    QCoro::QmlTask BookmarkDetail(Illustration* illust);
    QCoro::QmlTask Follow(User* user, bool privateFollow = false);
    QCoro::QmlTask RemoveFollow(User* user);
    QCoro::QmlTask FollowDetail(User* user);
    QCoro::QmlTask RelatedIllusts(Illustration* illust);
    QCoro::QmlTask SearchAutocomplete(QString query);
    QCoro::QmlTask SearchPopularPreview(SearchRequest* params);
    QCoro::QmlTask Search(SearchRequest* params);
    QCoro::QmlTask LatestGlobal(QString type);
    QCoro::QmlTask BookmarksFeed(QString type, QString restriction);

    QCoro::QmlTask RecommendedFeedNext(Recommended* recommended);
    QCoro::QmlTask FollowingFeedNext(Illusts* feed);
    QCoro::QmlTask SearchNext(SearchResults* results);
    QCoro::QmlTask LatestGlobalNext(Illusts* illusts);
    QCoro::QmlTask BookmarksFeedNext(Illusts* illusts);
};
