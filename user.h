#pragma once
#include <QtQmlIntegration>
#include "imageurls.h"

class User : public QObject
{
    Q_OBJECT
    QML_ELEMENT;

    QM_PROPERTY(int, id)
    QM_PROPERTY(QString, name)
    QM_PROPERTY(QString, account)
    QM_PROPERTY(ImageUrls*, profileImageUrls)
    QM_PROPERTY(int, isFollowed)
    QM_PROPERTY(bool, isAcceptRequest)

    public:
        User(QObject* parent = nullptr) : QObject(parent) {};
        User(QObject* parent, QJsonObject data) : QObject(parent)
        {
            m_id = data["id"].toInt();
            m_name = data["name"].toString();
            m_account = data["account"].toString();
            m_profileImageUrls = new ImageUrls(nullptr, data["profile_image_urls"].toObject());
            m_isFollowed = data["is_followed"].toBool(); // 0 - not followed, 1 - publicly followed, 2 - privately followed
            m_isAcceptRequest = data["is_accept_request"].toBool();
        };
};
class Account : public User
{
    Q_OBJECT
    QML_ELEMENT;

    QM_PROPERTY(bool, isMailAuthorized)
    QM_PROPERTY(bool, isPremium)
    QM_PROPERTY(QString, mailAddress)
    QM_PROPERTY(bool, requirePolicyAgreement)
    QM_PROPERTY(int, xRestrict)

    public:
        Account(QObject* parent = nullptr) : User(parent) {};
        Account(QObject* parent, QJsonObject data) : User(parent, data)
        {
            m_isMailAuthorized = data["is_mail_authorized"].toInt();
            m_isPremium = data["is_premium"].toBool();
            m_mailAddress = data["mail_address"].toString();
            m_requirePolicyAgreement = data["require_policy_agreement"].toBool();
            m_xRestrict = data["x_restrict"].toBool();
        };
};
