#include "comments.h"

Comments::Comments(QObject *parent) : QJObject(parent) {};
Comments::Comments(QObject *parent, QJsonObject data, QString accessToken, QString refreshToken)
    : QJObject(parent) {
    Q_UNUSED(accessToken);
    Q_UNUSED(refreshToken);
    deserialize(data);
}

void Comments::assignProperty(const QString &propertyName, const QJsonValue &data) {
    if (propertyName == "comments") {
        for (QJsonValue com : data["comments"].toArray()) {
            m_comments.append(new Comment(nullptr, com.toObject()));
        }
        Q_EMIT commentsChanged();
    } else if (propertyName == "next") {
        m_nextUrl = data.toString();
        Q_EMIT nextUrlChanged();
    } else
        QJObject::assignProperty(propertyName, data);
}
