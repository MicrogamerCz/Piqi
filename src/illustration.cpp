#include "illustration.h"
#include "requestworker.h"
#include <QCoro/Task>
#include <QCoro/QCoroTask>
#include <QJsonObject>

Illustration::Illustration(QObject* parent) : Work(parent) { }
Illustration::Illustration(QObject* parent, QJsonObject data)
    : Work(parent, data) {
    for (QJsonValue tool : data["tools"].toArray())
        m_tools.append(tool.toString());
    m_pageCount = data["page_count"].toInt();
    m_width = data["width"].toInt();
    m_height = data["height"].toInt();
    m_sanityLevel = data["sanity_level"].toInt();
    if (data.contains("series"))
        m_series = new WorkPrimitive(nullptr, data["series"].toObject());
    else
        m_series = nullptr;
    m_metaSinglePage = data["meta_single_page"].toObject()["original_image_url"].toString();
    for (QJsonValue metaPage : data["meta_pages"].toArray())
        m_metaPages.append(new ImageUrls(nullptr, metaPage.toObject()["image_urls"].toObject()));
    Q_EMIT metaPagesChanged();
    m_illustAiType = data["illust_ai_type"].toInt();
    m_illustBookType = data["illust_book_type"].toInt();
    if (data.contains("restriction_attributes")) {
        for (QJsonValue val : data["restriction_attributes"].toArray())
            m_restrictionAttributes.append(val.toString());
    }
    if (data.contains("comment_access_control"))
        m_commentAccessControl = data["comment_access_control"].toInt();
    m_totalComments = data["total_comments"].toInt();
}

const QString Illustration::type() {
    return "illust";
}

QCoro::QmlTask Illustration::FetchComments() { return FetchCommentsTask(); }
QCoro::Task<Comments*> Illustration::FetchCommentsTask() {
    QUrl url("https://app-api.pixiv.net/v3/illust/comments");
    QUrlQuery query{{"illust_id", QString::number(m_id)}};
    url.setQuery(query);
    return PiqiInternal::SendGet<Comments>(url);
}
