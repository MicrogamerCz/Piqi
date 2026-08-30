#pragma once
#include "illustration.h"

class PIQI_EXPORT SeriesDetail : public Work {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(ImageUrls *, coverImageUrls)
    QM_PROPERTY(int, seriesWorkCount) // shared with 'published_content_count'
    QM_PROPERTY(int, width)
    QM_PROPERTY(int, height)
    QM_PROPERTY(bool, watchlistAdded)

    // Additional (optional) properties (AFAIK) only in /v1/watchlist/manga endpoint
    // mask_text
    QM_PROPERTY(QDateTime, lastPublishedContentDatetime)
    QM_PROPERTY(int, latestContentId)

  public:
    SeriesDetail(QObject *parent = nullptr);
    SeriesDetail(QObject *parent, QJsonObject data);

    QCoro::Task<> watchlistAddTask();
    QCoro::Task<> watchlistDeleteTask();

    Q_SLOT QCoro::QmlTask watchlistAdd();
    Q_SLOT QCoro::QmlTask watchlistDelete();

  protected:
    const QString type() const override;

  private:
    const QStringList properties = {"coverImageUrls", "url", "publishedContentCount", "lastPublishedContentDatetime"};

    void assignProperty(const QString &propertyName, const QJsonValue &data) override;
};

class PIQI_EXPORT IllustSeriesContext : public QJObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(int, contentOrder)
    QM_PROPERTY(Illustration *, prev)
    QM_PROPERTY(Illustration *, next)

  public:
    IllustSeriesContext(QObject *parent = nullptr);
    IllustSeriesContext(QObject *parent, QJsonObject data);

  private:
    void assignProperty(const QString &propertyName, const QJsonValue &data) override;
};

class PIQI_EXPORT IllustSeries : public QObject // TODO: move to QJObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(SeriesDetail *, illustSeriesDetail)
    QM_PROPERTY(IllustSeriesContext *, illustSeriesContext)

  public:
    IllustSeries(QObject *parent = nullptr);
    IllustSeries(QObject *parent, QJsonObject data);
};

class PIQI_EXPORT SeriesDetails : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QList<SeriesDetail *>, series)
    QM_PROPERTY(QString, nextUrl)

  public:
    SeriesDetails(QObject *parent = nullptr) : QAbstractListModel(parent) {};
    SeriesDetails(QObject *parent, QJsonObject data) : QAbstractListModel(parent) {
        QJsonArray feed;
        if (data.contains("illust_series_details"))
            feed = data["illust_series_details"].toArray();
        else if (data.contains("series"))
            feed = data["series"].toArray();
        beginResetModel();
        for (const QJsonValue &sr : feed)
            m_series.append(new SeriesDetail(this, sr.toObject()));
        endResetModel();
        if (data.keys().contains("next_url"))
            m_nextUrl = data["next_url"].toString();
        else
            m_nextUrl = "";
    }
    Q_SLOT void extend(SeriesDetails *nextFeed) {
        m_nextUrl = nextFeed->m_nextUrl;
        Q_EMIT nextUrlChanged();

        beginInsertRows({}, m_series.count(), m_series.count() + nextFeed->m_series.count() - 1);
        m_series.append(nextFeed->m_series);
        endInsertRows();

        Q_EMIT seriesChanged();
    }

    enum CustomRoles {
        SeriesRole = Qt::UserRole,
    };
    Q_ENUM(CustomRoles)

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};
