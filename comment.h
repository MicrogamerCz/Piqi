#pragma once
#include "stamp.h"
#include "user.h"

class Comment : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(int, id)
    QM_PROPERTY(QString, comment)
    QM_PROPERTY(QDateTime, date)
    QM_PROPERTY(User *, user)
    QM_PROPERTY(bool, hasReplies)
    QM_PROPERTY(Stamp *, stamp)

public:
    Comment(QObject *parent = nullptr)
        : QObject(parent) { };
    Comment(QObject *parent, QJsonObject data)
        : QObject(parent)
    {
        m_id = data["id"].toInt();
        m_comment = data["comment"].toString();
        m_date = QDateTime::fromString(data["date"].toString(), Qt::ISODate);
        m_user = new User(this, data["user"].toObject());
        m_hasReplies = data["has_replies"].toBool();

        QJsonObject stampObject = data["stamp"].toObject();
        m_stamp = !stampObject.isEmpty() ? new Stamp(nullptr, stampObject) : nullptr;
    }
};
