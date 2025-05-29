#pragma once
#include "series.h"
#include "tag.h"
#include "user.h"

class Illustration : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(int, id)
    QM_PROPERTY(QString, title)
    QM_PROPERTY(QString, type)
    QM_PROPERTY(ImageUrls *, imageUrls)
    QM_PROPERTY(QString, caption)
    QM_PROPERTY(int, restricted)
    QM_PROPERTY(User *, user)
    QM_PROPERTY(QList<Tag *>, tags)
    QM_PROPERTY(QStringList, tools)
    QM_PROPERTY(QDateTime, createDate)
    QM_PROPERTY(int, pageCount)
    QM_PROPERTY(int, width)
    QM_PROPERTY(int, height)
    QM_PROPERTY(int, sanityLevel)
    QM_PROPERTY(int, xRestrict)
    QM_PROPERTY(Series *, series)
    QM_PROPERTY(QString, metaSinglePage)
    QM_PROPERTY(QList<ImageUrls *>, metaPages)
    QM_PROPERTY(int, totalView)
    QM_PROPERTY(int, totalBookmarks)
    QM_PROPERTY(int, isBookmarked) // 0 = none, 1 = public, 2 = private
    QM_PROPERTY(bool, visible)
    QM_PROPERTY(bool, isMuted)
    QM_PROPERTY(int, illustAiType)
    QM_PROPERTY(int, illustBookType)

public:
    Illustration(QObject *parent = nullptr);
    Illustration(QObject *parent, QJsonObject data);
};
