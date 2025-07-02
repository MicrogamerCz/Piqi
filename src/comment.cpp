#include <qobject.h>
#include "comment.h"

Comment::Comment(QObject* parent) : QObject(parent) {}

Comment::Comment(QObject *parent, QJsonObject data) : QObject(parent)
{
    m_id = data["id"].toInt();
    m_comment = data["comment"].toString();
    m_date = QDateTime::fromString(data["date"].toString(), Qt::ISODate);
    m_user = new User(this, data["user"].toObject());
    m_hasReplies = data["has_replies"].toBool();

    QJsonObject stampObject = data["stamp"].toObject();
    m_stamp = !stampObject.isEmpty() ? new Stamp(nullptr, stampObject) : nullptr;
}
