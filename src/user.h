#pragma once
#include <QtQmlIntegration>
#include "imageurls.h"
#include "piqi_export.h"

class PIQI_EXPORT User : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(int, id)
    QM_PROPERTY(QString, name)
    QM_PROPERTY(QString, account)
    QM_PROPERTY(ImageUrls*, profileImageUrls)
    QM_PROPERTY(int, isFollowed)
    QM_PROPERTY(bool, isAcceptRequest)

public:
    User(QObject* parent = nullptr);
    User(QObject* parent, QJsonObject data);
};

class PIQI_EXPORT Account : public User
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(bool, isMailAuthorized)
    QM_PROPERTY(bool, isPremium)
    QM_PROPERTY(QString, mailAddress)
    QM_PROPERTY(bool, requirePolicyAgreement)
    QM_PROPERTY(int, xRestrict)

public:
    Account(QObject* parent = nullptr);
    Account(QObject* parent, QJsonObject data);
};
