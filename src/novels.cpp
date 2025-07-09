#include "novels.h"
#include "novel.h"
#include <qabstractitemmodel.h>
#include <qjsonobject.h>
#include <qobject.h>

Novels::Novels(QObject* parent) : QAbstractListModel(parent) { }
Novels::Novels(QObject* parent, QJsonObject data) : QAbstractListModel(parent) {
    beginResetModel();
    for (QJsonValue nl : data["novels"].toArray()) {
        Novel* novel = new Novel(nullptr, nl.toObject());
        novels.append(novel);
    }
    endResetModel();
    if (data.keys().contains("next_url")) m_nextUrl = data["next_url"].toString();
    else m_nextUrl = "";
}
void Novels::Extend(Novels* nextFeed) {
    m_nextUrl = nextFeed->m_nextUrl;
    Q_EMIT nextUrlChanged();

    beginInsertRows({}, novels.count(), novels.count() + nextFeed->novels.count() - 1);
    novels.append(nextFeed->novels);
    endInsertRows();

    // Q_EMIT illustsChanged();
}
int Novels::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return novels.count();
}
QVariant Novels::data(const QModelIndex &index, int role) const
{
    const auto novel = novels[index.row()];
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
    for (QJsonValue nl : data["ranking_novels"].toArray()) {
        Novel* novel = new Novel(nullptr, nl.toObject());
        m_rankingNovels.append(novel);
    }
    m_privacyPolicy = new PrivacyPolicy(nullptr, data["privacy_policy"].toObject());
    m_contestExists = data["contest_exists"].toBool();
}
