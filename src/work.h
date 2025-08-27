#pragma once
#include "bookmarkdetails.h"
#include "imageurls.h"
#include "piqi_export.h"
#include "tag.h"
#include "user.h"
#include <qcoroqmltask.h>
#include <qcorotask.h>
#include <qtmetamacros.h>

class PIQI_EXPORT WorkPrimitive : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(int, id)
    QM_PROPERTY(QString, title)

public:
    WorkPrimitive(QObject* parent = nullptr);
    WorkPrimitive(QObject* parent, QJsonObject data);
};

class PIQI_EXPORT Work : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(int, id)
    QM_PROPERTY(QString, title)
    QM_PROPERTY(QString, caption)
    QM_PROPERTY(int, restricted)
    QM_PROPERTY(ImageUrls*, imageUrls)
    QM_PROPERTY(User*, user)
    QM_PROPERTY(QList<Tag *>, tags)
    QM_PROPERTY(QDateTime, createDate)
    QM_PROPERTY(int, pageCount)
    QM_PROPERTY(int, xRestrict)
    QM_PROPERTY(int, isBookmarked) // 0 = none, 1 = public, 2 = private
    QM_PROPERTY(bool, visible)
    QM_PROPERTY(bool, isMuted)
    QM_PROPERTY(int, totalBookmarks)
    QM_PROPERTY(int, totalView)

    protected:
        virtual const QString type() = 0;

public:
    Work(QObject *parent = nullptr);
    Work(QObject *parent, QJsonObject data);

    QCoro::Task<void> AddBookmarkTask(bool isPrivate = false);
    QCoro::Task<void> RemoveBookmarkTask();
    QCoro::Task<BookmarkDetails*> BookmarkDetailTask();

public Q_SLOTS:
    QCoro::QmlTask AddBookmark(bool isPrivate = false);
    QCoro::QmlTask RemoveBookmark();
    QCoro::QmlTask BookmarkDetail();
};
