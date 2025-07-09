#include "illustration.h"
#include <qjsonobject.h>
#include <qobject.h>

Illustration::Illustration(QObject* parent) : Work(parent) { }
Illustration::Illustration(QObject* parent, QJsonObject data) : Work(parent, data) {
    for (QJsonValue tool : data["tools"].toArray())
        m_tools.append(tool.toString());
    m_pageCount = data["page_count"].toInt();
    m_width = data["width"].toInt();
    m_height = data["height"].toInt();
    m_sanityLevel = data["sanity_level"].toInt();
    if (data.contains("series"))
        m_series = new Series(nullptr, data["series"].toObject());
    m_metaSinglePage = data["meta_single_page"].toObject()["original_image_url"].toString();
    for (QJsonValue metaPage : data["meta_pages"].toArray())
        m_metaPages.append(new ImageUrls(nullptr, metaPage.toObject()["image_urls"].toObject()));
    Q_EMIT metaPagesChanged();
    m_illustAiType = data["illust_ai_type"].toInt();
    m_illustBookType = data["illust_book_type"].toInt();
}
