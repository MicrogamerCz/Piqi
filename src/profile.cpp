#include "profile.h"
#include <qjsonobject.h>
#include <qobject.h>
#include <qtmetamacros.h>

Profile::Profile(QObject* parent) : QObject(parent) {}
Profile::Profile(QObject* parent, QJsonObject data) : QObject(parent) {
    m_webpage = data["webpage"].toString();
    m_gender = data.contains("gender") ? data["gender"].toInt() : -1;
    m_birth = data["birth"].toString();
    m_birthDay = data["birth_day"].toString();
    m_birthYear = data["birth_year"].toInt();
    m_region = data["region"].toString();
    m_addressId = data["address_id"].toInt();
    m_countryCode = data["country_code"].toString();
    m_job = data["job"].toString();
    m_jobId = data["job_id"].toInt();
    m_totalFollowUsers = data["total_follow_users"].toInt();
    m_totalMyPixivUsers = data["total_my_pixiv_users"].toInt();
    m_totalIllusts = data["total_illusts"].toInt();
    m_totalManga = data["total_manga"].toInt();
    m_totalNovels = data["total_novels"].toInt();
    m_totalIllustSeries = data["total_illust_series"].toInt();
    m_totalNovelSeries = data["total_novel_series"].toInt();
    m_backgroundImageUrl = data["background_image_url"].toString();
    Q_EMIT backgroundImageUrlChanged();
    // twitter_account
    m_twitterUrl = data["twitter_url"].toString();
    m_isPremium = data["is_premium"].toBool();
    m_isUsingCustomProfileImage = data["is_using_custom_profile_image"].toBool();
}
