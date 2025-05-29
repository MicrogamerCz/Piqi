#include "user.h"

User::User(QObject* parent) : QObject(parent) {}

User::User(QObject* parent, QJsonObject data) : QObject(parent)
{
    m_id = data["id"].toInt();
    m_name = data["name"].toString();
    m_account = data["account"].toString();
    m_profileImageUrls = new ImageUrls(nullptr, data["profile_image_urls"].toObject());
    m_isFollowed = data["is_followed"].toBool(); // 0 - not followed, 1 - publicly followed, 2 - privately followed
    m_isAcceptRequest = data["is_accept_request"].toBool();
}

Account::Account(QObject* parent) : User(parent) {}

Account::Account(QObject* parent, QJsonObject data) : User(parent, data)
{
    m_isMailAuthorized = data["is_mail_authorized"].toInt();
    m_isPremium = data["is_premium"].toBool();
    m_mailAddress = data["mail_address"].toString();
    m_requirePolicyAgreement = data["require_policy_agreement"].toBool();
    m_xRestrict = data["x_restrict"].toBool();
}
