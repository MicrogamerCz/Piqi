#include "novels.h"
#include "novel.h"
#include <qabstractitemmodel.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qobject.h>
#include <qtpreprocessorsupport.h>

Novels::Novels(QObject* parent) : QAbstractListModel(parent) { }
Novels::Novels(QObject* parent, QJsonObject data) : QAbstractListModel(parent)
{
    beginResetModel();
    for (QJsonValue nl : data["novels"].toArray()) {
        Novel* novel = new Novel(nullptr, nl.toObject());
        m_novels.append(novel);
    }
    endResetModel();
    if (data.keys().contains("next_url")) m_nextUrl = data["next_url"].toString();
    else m_nextUrl = "";
}
void Novels::Extend(Novels* nextFeed) {
    m_nextUrl = nextFeed->m_nextUrl;
    Q_EMIT nextUrlChanged();

    beginInsertRows({}, m_novels.count(), m_novels.count() + nextFeed->m_novels.count() - 1);
    m_novels.append(nextFeed->m_novels);
    endInsertRows();
}
int Novels::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_novels.count();
}
QVariant Novels::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)

    const auto novel = m_novels[index.row()];
    return QVariant::fromValue(novel);
}
QHash<int, QByteArray> Novels::roleNames() const
{
    return {
        {CustomRoles::NovelRole, QByteArrayLiteral("novel")},
    };
}

RecommendedNovels::RecommendedNovels(QObject* parent) : Novels(parent) { }
RecommendedNovels::RecommendedNovels(QObject* parent, QJsonObject data) : Novels(parent, data) {
    m_ranking = new Novels;
    for (QJsonValue nl : data["ranking_novels"].toArray()) {
        Novel* novel = new Novel(nullptr, nl.toObject());
        m_ranking->m_novels.append(novel);
    }
    m_privacyPolicy = new PrivacyPolicy(nullptr, data["privacy_policy"].toObject());
    m_contestExists = data["contest_exists"].toBool();
}

NovelSearchResults::NovelSearchResults(QObject* parent) : Novels(parent) {};
NovelSearchResults::NovelSearchResults(QObject* parent, QJsonObject data) : Novels(parent, data)
{
    m_showAi = data["show_ai"].toBool();
};
