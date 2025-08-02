#include "series.h"
#include "illustration.h"
#include <qjsonobject.h>
#include <qobject.h>

SeriesDetail::SeriesDetail(QObject* parent) : Work(parent) {}
SeriesDetail::SeriesDetail(QObject* parent, QJsonObject data) : Work(parent, data)
{
    m_coverImageUrls = new ImageUrls(this, data["cover_image_urls"].toObject());
    if (data.contains("series_work_count")) m_seriesWorkCount = data["series_work_count"].toInt();
    else if (data.contains("published_content_count")) m_seriesWorkCount = data["published_content_count"].toInt();
    if (data.contains("width")) m_width = data["width"].toInt();
    if (data.contains("height")) m_height = data["height"].toInt();
    if (data.contains("watchlist_added")) m_watchlistAdded = data["watchlist_added"].toBool();

    if (data.contains("last_published_content_datetime")) m_lastPublishedContentDatetime = QDateTime::fromString(data["last_published_content_datetime"].toString(), Qt::ISODateWithMs);
    if (data.contains("latest_content_id")) m_latestContentId = data["latest_content_id"].toInt();
}

IllustSeriesContext::IllustSeriesContext(QObject* parent) : QObject(parent) {}
IllustSeriesContext::IllustSeriesContext(QObject* parent, QJsonObject data) : QObject(parent)
{
    m_contentOrder = data["content_order"].toInt();
    if (data.contains("prev")) m_prev = new Illustration(nullptr, data["prev"].toObject());
    else m_prev = nullptr;
    if (data.contains("next")) m_next = new Illustration(nullptr, data["next"].toObject());
    else m_next = nullptr;
}

IllustSeries::IllustSeries(QObject* parent) : QObject(parent) {}
IllustSeries::IllustSeries(QObject* parent, QJsonObject data) : QObject(parent)
{
    m_illustSeriesDetail = new SeriesDetail(nullptr, data["illust_series_detail"].toObject());
    m_illustSeriesContext = new IllustSeriesContext(nullptr, data["illust_series_context"].toObject());
}

int SeriesDetails::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_series.count();
}

QVariant SeriesDetails::data(const QModelIndex &index, int role) const
{
    const auto illustration = m_series[index.row()];
    if (role == CustomRoles::SeriesRole) {
        return QVariant::fromValue(illustration);
    }
    return {};
}

QHash<int, QByteArray> SeriesDetails::roleNames() const
{
    return {
        {CustomRoles::SeriesRole, QByteArrayLiteral("series")},
    };
}
