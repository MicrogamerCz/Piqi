#include <qobject.h>
#include "comment.h"

Comment::Comment(QObject *parent) : QJObject(parent) {
}

Comment::Comment(QObject *parent, QJsonObject data) : QJObject(data, parent) {
}

void Comment::assignProperty(const QString &propertyName, const QJsonValue &data) {
    if (propertyName == "date") {
        m_date = QDateTime::fromString(data.toString(), Qt::ISODate);
        Q_EMIT dateChanged();
    } else if (propertyName == "user") {
        m_user = new User(this, data.toObject());
        Q_EMIT userChanged();
    } else if (propertyName == "stamp") {
        m_stamp = new Stamp(nullptr, data.toObject());
        Q_EMIT stampChanged();
    } else
        QJObject::assignProperty(propertyName, data);
}
