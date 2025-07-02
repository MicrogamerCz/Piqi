#pragma once
#include <QJsonArray>
#include "piqi_export.h"
#include "tag.h"

class PIQI_EXPORT BookmarkDetails : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(bool, isBookmarked)
    QM_PROPERTY(QList<BookmarkTag *>, tags)
    QM_PROPERTY(QString, restriction)

public:
    BookmarkDetails(QObject *parent = nullptr);
    BookmarkDetails(QObject *parent, QJsonObject data);
};
class FollowDetails : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(bool, isFollowed)
    QM_PROPERTY(QString, restriction)

public:
    FollowDetails(QObject *parent = nullptr);
    FollowDetails(QObject *parent, QJsonObject data);
};
