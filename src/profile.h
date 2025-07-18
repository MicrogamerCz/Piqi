#pragma once
#include "piqi_export.h"
#include "qepr.h"
#include <qjsonobject.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class PIQI_EXPORT Profile : public QObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, webpage)
    QM_PROPERTY(int, gender)
    QM_PROPERTY(QString, birth)
    QM_PROPERTY(QString, birthDay)
    QM_PROPERTY(int, birthYear)
    QM_PROPERTY(QString, region)
    QM_PROPERTY(int, addressId)
    QM_PROPERTY(QString, countryCode)
    QM_PROPERTY(QString, job)
    QM_PROPERTY(int, jobId)
    QM_PROPERTY(int, totalFollowUsers)
    QM_PROPERTY(int, totalMyPixivUsers)
    QM_PROPERTY(int, totalIllusts)
    QM_PROPERTY(int, totalManga)
    QM_PROPERTY(int, totalNovels)
    QM_PROPERTY(int, totalIllustSeries)
    QM_PROPERTY(int, totalNovelSeries)
    QM_PROPERTY(QString, backgroundImageUrl) // profile banner
    // twitter_account
    QM_PROPERTY(QString, twitterUrl)
    QM_PROPERTY(bool, isPremium)
    QM_PROPERTY(bool, isUsingCustomProfileImage)

    public:
        Profile(QObject* parent = nullptr);
        Profile(QObject* parent, QJsonObject data);
};
