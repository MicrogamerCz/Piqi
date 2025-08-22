#pragma once
#include "imageurls.h"
#include "novel.h"
#include "piqi_export.h"
#include "privacypolicy.h"
#include "tag.h"
#include <qabstractitemmodel.h>
#include <qjsonobject.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class PIQI_EXPORT Novels : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, nextUrl)
    QM_PROPERTY(QList<Novel*>, novels)

    public:
        Novels(QObject* parent = nullptr);
        Novels(QObject* parent, QJsonObject data, QString accessToken = "", QString refreshToken = "");

        Q_SLOT void Extend(Novels* nextFeed);

        enum CustomRoles {
            NovelRole = Qt::UserRole,
        };
        Q_ENUM(CustomRoles)

        int rowCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        QHash<int, QByteArray> roleNames() const override;
};
class PIQI_EXPORT RecommendedNovels : public Novels
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(PrivacyPolicy*, privacyPolicy)
    QM_PROPERTY(Novels*, ranking)
    QM_PROPERTY(bool, contestExists)

    public:
        RecommendedNovels(QObject* parent = nullptr);
        RecommendedNovels(QObject* parent, QJsonObject data, QString accessToken = "", QString refreshToken = "");
};

class NovelSearchResults : public Novels
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(bool, showAi)

    public:
        NovelSearchResults(QObject* parent = nullptr);
        NovelSearchResults(QObject* parent, QJsonObject data, QString accessToken = "", QString refreshToken = "");
};
