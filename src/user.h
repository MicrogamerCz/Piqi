#pragma once
#include "bookmarkdetails.h"
#include "imageurls.h"
#include <QCoroQml>
#include <qtmetamacros.h>

class PIQI_EXPORT User : public QJObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(int, id)
    QM_PROPERTY(QString, name)
    QM_PROPERTY(QString, account)
    QM_PROPERTY(QString, comment)
    QM_PROPERTY(ImageUrls *, profileImageUrls)
    QM_PROPERTY(int, isFollowed)
    QM_PROPERTY(bool, isAcceptRequest)

  public:
    User(QObject *parent = nullptr);
    User(QObject *parent, QJsonObject data);

    Q_INVOKABLE QCoro::QmlTask follow(bool privateFollow = false);
    QCoro::Task<> followTask(bool privateFollow = false);

    Q_INVOKABLE QCoro::QmlTask removeFollow();
    QCoro::Task<> removeFollowTask();

    Q_INVOKABLE QCoro::QmlTask followDetail();
    QCoro::Task<FollowDetails *> followDetailTask();

  private:
    void assignProperty(const QString &propertyName, const QJsonValue &data) override;
};

class PIQI_EXPORT Account : public User {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(bool, isMailAuthorized)
    QM_PROPERTY(bool, isPremium)
    QM_PROPERTY(QString, mailAddress)
    QM_PROPERTY(bool, requirePolicyAgreement)
    QM_PROPERTY(int, xRestrict)

  public:
    Account(QObject *parent = nullptr);
    Account(QObject *parent, QJsonObject data);
};
