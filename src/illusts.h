#pragma once
#include "piqi_export.h"
#include "privacypolicy.h"
#include "illustration.h"

class PIQI_EXPORT Illusts : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QList<Illustration*>, illusts)
    QM_PROPERTY(QString, nextUrl)

public:
    Illusts(QObject* parent = nullptr) : QAbstractListModel(parent) {};
    Illusts(QObject* parent, QJsonObject data) : QAbstractListModel(parent)
    {
        beginResetModel();
        for (QJsonValue il : data["illusts"].toArray()) {
            Illustration* illust = new Illustration(nullptr, il.toObject());
            m_illusts.append(illust);
        }
        endResetModel();
        if (data.keys().contains("next_url")) m_nextUrl = data["next_url"].toString();
        else m_nextUrl = "";
    }
    Q_SLOT void Extend(Illusts* nextFeed) {
        m_nextUrl = nextFeed->m_nextUrl;

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

class Recommended : public Illusts
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(PrivacyPolicy*, privacyPolicy)
    QM_PROPERTY(QList<Illustration*>, rankingIllusts)
    QM_PROPERTY(bool, contestExists)

        public:
            Recommended(QObject* parent = nullptr) : Illusts(parent) {};
            Recommended(QObject* parent, QJsonObject data) : Illusts(parent, data)
            {
                m_privacyPolicy = new PrivacyPolicy(nullptr, data["privacy_policy"].toObject());
                for (QJsonValue il : data["ranking_illusts"].toArray()) {
                    Illustration* illust = new Illustration(nullptr, il.toObject());
                    m_rankingIllusts.append(illust);
                }
                m_contestExists = data["contest_exists"].toBool();
            };
};

class SearchResults : public Illusts
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
