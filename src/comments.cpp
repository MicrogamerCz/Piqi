#include "comments.h"

Comments::Comments(QObject *parent) : QJObject(parent) {};
Comments::Comments(QObject *parent, QJsonObject data) : QJObject(parent) {
    deserialize(data);
}

void Comments::assignProperty(const QString &propertyName, const QJsonValue &data) {
    if (propertyName == "comments") {
        for (const QJsonValue &com : data["comments"].toArray())
            m_comments.append(new Comment(this, com.toObject()));
        Q_EMIT commentsChanged();
    } else if (propertyName == "next") {
        m_nextUrl = data.toString();
        Q_EMIT nextUrlChanged();
    } else
        QJObject::assignProperty(propertyName, data);
}
