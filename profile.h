#pragma once
#include "qepr.h"
#include <qjsonobject.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class Profile : public QObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, webpage)
    // gender
    // birth
    // birth_day
    // birth_year
    QM_PROPERTY(QString, region)
    QM_PROPERTY(int, addressId)
    QM_PROPERTY(QString, countryCode)
    // job
    // job_id
    QM_PROPERTY(int, totalFollowUsers)
    QM_PROPERTY(int, totalMyPixivUsers)
    QM_PROPERTY(int, totalIllusts)
    QM_PROPERTY(int, totalManga)
    QM_PROPERTY(int, totalNovels)
    QM_PROPERTY(int, totalIllustSeries)
    QM_PROPERTY(int, totalNovelSeries)
    QM_PROPERTY(QString, backgroundImageUrl) // profile banner
    // twitter_account
    // twitter_url
    QM_PROPERTY(bool, isPremium)
    QM_PROPERTY(bool, isUsingCustomProfileImage)

    public:
        Profile(QObject* parent = nullptr);
        Profile(QObject* parent, QJsonObject data);
};
