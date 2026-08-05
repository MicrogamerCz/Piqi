#include "series.h"
#include "requestworker.h"

SeriesDetail::SeriesDetail(QObject *parent) : Work(parent) {
}
SeriesDetail::SeriesDetail(QObject *parent, QJsonObject data) : Work(parent) {
    deserialize(data);
}
const QString SeriesDetail::type() const {
    return "manga"; // default for now
}
QCoro::QmlTask SeriesDetail::WatchlistAdd() {
    return WatchlistAddTask();
}
QCoro::Task<> SeriesDetail::WatchlistAddTask() {
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v1/watchlist/manga/add"));
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery query{
        {"series_id", QString::number(m_id)},
    };
    co_await PiqiInternal::manager.post(request, query.toString().toUtf8());
}
QCoro::QmlTask SeriesDetail::WatchlistDelete() {
    return WatchlistDeleteTask();
}
QCoro::Task<> SeriesDetail::WatchlistDeleteTask() {
    QNetworkRequest request(QUrl("https://app-api.pixiv.net/v1/watchlist/manga/delete"));
    request.setRawHeader("Authorization", ("Bearer " + PiqiInternal::accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery query{
        {"series_id", QString::number(m_id)},
    };
    co_await PiqiInternal::manager.post(request, query.toString().toUtf8());
}
void SeriesDetail::assignProperty(const QString &propertyName, const QJsonValue &data) {
    switch (properties.indexOf(propertyName)) {
    case 0: // coverImageUrls
        m_coverImageUrls = new ImageUrls(this, data.toObject());
        Q_EMIT coverImageUrlsChanged();
        break;
    case 1: // url
        m_coverImageUrls = new ImageUrls(this);
        m_coverImageUrls->m_medium = data.toString(); // why, pixiv, why?
        Q_EMIT coverImageUrlsChanged();
        break;
    case 2:                               // publishedContentCount
        m_seriesWorkCount = data.toInt(); // TODO: add property aliases to QJObject
        Q_EMIT seriesWorkCountChanged();
        break;
    case 3: // lastPublishedContentDatetime
        m_lastPublishedContentDatetime = QDateTime::fromString(data.toString(), Qt::ISODateWithMs);
        Q_EMIT lastPublishedContentDatetimeChanged();
        break;
    default:
        Work::assignProperty(propertyName, data);
    }
}

IllustSeriesContext::IllustSeriesContext(QObject *parent) : QJObject(parent) {
}
IllustSeriesContext::IllustSeriesContext(QObject *parent, QJsonObject data) : QJObject(parent) {
    deserialize(data);
}
void IllustSeriesContext::assignProperty(const QString &propertyName, const QJsonValue &data) {
    if (propertyName == "prev") {
        m_prev = new Illustration(this, data.toObject());
        Q_EMIT prevChanged();
    } else if (propertyName == "next") {
        m_next = new Illustration(this, data.toObject());
        Q_EMIT nextChanged();
    } else
        QJObject::assignProperty(propertyName, data);
}

IllustSeries::IllustSeries(QObject *parent) : QObject(parent) {
}
IllustSeries::IllustSeries(QObject *parent, QJsonObject data) : QObject(parent) {
    m_illustSeriesDetail = new SeriesDetail(this, data["illust_series_detail"].toObject());
    m_illustSeriesContext = new IllustSeriesContext(this, data["illust_series_context"].toObject());
}

int SeriesDetails::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_series.count();
}
QVariant SeriesDetails::data(const QModelIndex &index, int role) const {
    const auto illustration = m_series[index.row()];
    if (role == CustomRoles::SeriesRole) {
        return QVariant::fromValue(illustration);
    }
    return {};
}
QHash<int, QByteArray> SeriesDetails::roleNames() const {
    return {
        {CustomRoles::SeriesRole, QByteArrayLiteral("series")},
    };
}
