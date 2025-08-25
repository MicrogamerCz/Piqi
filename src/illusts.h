#pragma once
#include "piqi_export.h"
#include "privacypolicy.h"
#include "illustration.h"
#include "series.h"
#include <qcoroqmltask.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class PIQI_EXPORT Illusts : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QList<Illustration*>, illusts)
    QM_PROPERTY(QString, nextUrl)

public:
    Illusts(QObject* parent);
    Illusts(QObject* parent, QJsonObject data);

    Q_SLOT QCoro::QmlTask NextFeed();
    QCoro::Task<> NextFeedTask();

    Q_SLOT void Extend(Illusts* nextFeed) {
        m_nextUrl = nextFeed->m_nextUrl;
        Q_EMIT nextUrlChanged();

        beginInsertRows({}, m_illusts.count(), m_illusts.count() + nextFeed->m_illusts.count() - 1);
        m_illusts.append(nextFeed->m_illusts);
        endInsertRows();

        Q_EMIT illustsChanged();
    }

    enum CustomRoles {
        IllustRole = Qt::UserRole,
    };
    Q_ENUM(CustomRoles)

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

class PIQI_EXPORT Recommended : public Illusts
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(PrivacyPolicy*, privacyPolicy)
    QM_PROPERTY(Illusts*, ranking)
    QM_PROPERTY(bool, contestExists)

    public:
        Recommended(QObject* parent = nullptr) : Illusts(parent) {};
        Recommended(QObject* parent, QJsonObject data, QString accessToken = "", QString refreshToken = "") : Illusts(parent, data)
        {
            m_ranking = new Illusts(nullptr);

            for (QJsonValue il : data["ranking_illusts"].toArray()) {
                Illustration* illust = new Illustration(nullptr, il.toObject());
                m_ranking->m_illusts.append(illust);
            }

            m_privacyPolicy = new PrivacyPolicy(nullptr, data["privacy_policy"].toObject());
            m_contestExists = data["contest_exists"].toBool();
        };
};

class PIQI_EXPORT SearchResults : public Illusts
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(bool, showAi)

    public:
        SearchResults(QObject* parent = nullptr) : Illusts(parent) {};
        SearchResults(QObject* parent, QJsonObject data) : Illusts(parent, data)
        {
            m_showAi = data["show_ai"].toBool();
        };
};

class PIQI_EXPORT Series : public Illusts
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(SeriesDetail*, illustSeriesDetail)
    QM_PROPERTY(Illustration*, illustSeriesFirstIllust)

    public:
        Series(QObject* parent = nullptr) : Illusts(parent) {};
        Series(QObject* parent, QJsonObject data) : Illusts(parent, data)
        {
            m_illustSeriesDetail = new SeriesDetail(nullptr, data["illust_series_detail"].toObject());
            m_illustSeriesFirstIllust = new Illustration(nullptr, data["illust_series_first_illust"].toObject());
        };
};
