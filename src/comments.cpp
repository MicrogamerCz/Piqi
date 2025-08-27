#include "comments.h"

Comments::Comments(QObject *parent) : QObject(parent) { };
Comments::Comments(QObject *parent, QJsonObject data, QString accessToken, QString refreshToken) : QObject(parent)
{
    Q_UNUSED(accessToken);
    Q_UNUSED(refreshToken);

    for (QJsonValue com : data["comments"].toArray()) {
        m_comments.append(new Comment(nullptr, com.toObject()));
    }
    m_nextUrl = data["next"].toString();
    if (data.keys().contains("comment_access_control")) m_commentAccessControl = data["comment_access_control"].toInt();
    else m_commentAccessControl = 0;
}
