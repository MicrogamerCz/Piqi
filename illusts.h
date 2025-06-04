#pragma once
#include "privacypolicy.h"
#include "illustration.h"

class Illusts : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QList<Illustration*>, illusts)
    QM_PROPERTY(QString, nextUrl)

    public:
        Illusts(QObject* parent = nullptr) : QObject(parent) {};
        Illusts(QObject* parent, QJsonObject data) : QObject(parent)
        {
            for (QJsonValue il : data["illusts"].toArray()) {
                Illustration* illust = new Illustration(nullptr, il.toObject());
                m_illusts.append(illust);
            }
            if (data.keys().contains("next_url")) m_nextUrl = data["next_url"].toString();
            else m_nextUrl = "";
        };
        Q_SLOT void Extend(Illusts* nextFeed) {
            m_nextUrl = nextFeed->m_nextUrl;
            m_illusts.append(nextFeed->m_illusts);
            Q_EMIT illustsChanged();
        };
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
