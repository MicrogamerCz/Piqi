#include "comment.h"
#include <qobject.h>

Comment::Comment(QObject *parent) : QJObject(parent), m_user(nullptr), m_stamp(nullptr) {
}
Comment::Comment(QObject *parent, QJsonObject data) : QJObject(parent), m_user(nullptr), m_stamp(nullptr) {
    deserialize(data);
}
void Comment::assignProperty(const QString &propertyName, const QJsonValue &data) {
    if (propertyName == "date") {
        m_date = QDateTime::fromString(data.toString(), Qt::ISODate);
        Q_EMIT dateChanged();
    } else if (propertyName == "user") {
        m_user = new User(this, data.toObject());
        Q_EMIT userChanged();
    } else if (propertyName == "stamp") {
        if (!data.isNull())
            m_stamp = new Stamp(this, data.toObject());
        Q_EMIT stampChanged();
    } else
        QJObject::assignProperty(propertyName, data);
}
