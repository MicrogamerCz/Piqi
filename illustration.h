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
    Illustration(QObject *parent = nullptr)
        : QObject(parent) { };
    Illustration(QObject *parent, QJsonObject data)
        : QObject(parent)
    {
        m_id = data["id"].toInt();
        m_title = data["title"].toString();
        m_imageUrls = new ImageUrls(nullptr, data["image_urls"].toObject());
        m_caption = data["caption"].toString();
        m_restricted = data["restrict"].toInt();
        m_user = new User(nullptr, data["user"].toObject());
        for (QJsonValue tag : data["tags"].toArray())
            m_tags.append(new Tag(nullptr, tag.toObject()));
        for (QJsonValue tool : data["tools"].toArray())
            m_tools.append(tool.toString());
        m_createDate = QDateTime::fromString(data["create_date"].toString(), Qt::ISODateWithMs);
        Q_EMIT createDateChanged();
        m_pageCount = data["page_count"].toInt();
        m_width = data["width"].toInt();
        m_height = data["height"].toInt();
        m_sanityLevel = data["sanity_level"].toInt();
        m_xRestrict = data["x_restrict"].toInt();
        if (data.contains("series"))
            m_series = new Series(nullptr, data["series"].toObject());
        m_metaSinglePage = data["meta_single_page"].toObject()["original_image_url"].toString();
        for (QJsonValue metaPage : data["meta_pages"].toArray())
            m_metaPages.append(new ImageUrls(nullptr, metaPage.toObject()["image_urls"].toObject()));
        Q_EMIT metaPagesChanged();
        m_totalView = data["total_view"].toInt();
        m_totalBookmarks = data["total_bookmarks"].toInt();
        m_isBookmarked = data["is_bookmarked"].toBool();
        m_visible = data["visible"].toBool();
        m_isMuted = data["is_muted"].toBool();
        m_illustAiType = data["illust_ai_type"].toInt();
        m_illustBookType = data["illust_book_type"].toInt();
    };
};
