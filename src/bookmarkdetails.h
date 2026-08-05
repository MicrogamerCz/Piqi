#pragma once
#include "tag.h"

class PIQI_EXPORT BookmarkDetails : public QJObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(bool, isBookmarked)
    QM_PROPERTY(QList<BookmarkTag *>, tags)
    QM_PROPERTY(QString, restrict)

  public:
    BookmarkDetails(QObject *parent = nullptr);
    BookmarkDetails(QObject *parent, QJsonObject data, QString accessToken = "", QString refreshToken = "");

  private:
    void assignProperty(const QString &propertyName, const QJsonValue &data) override;
};
class PIQI_EXPORT FollowDetails : public QJObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(bool, isFollowed)
    QM_PROPERTY(QString, restrict)

  public:
    FollowDetails(QObject *parent = nullptr);
    FollowDetails(QObject *parent, QJsonObject data);
};
