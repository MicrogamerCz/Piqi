#pragma once
#include "comment.h"
#include "qepr.h"

class Comments : public QObject
{
    Q_OBJECT
    QML_ELEMENT;

    QM_PROPERTY(QList<Comment *>, comments)
    QM_PROPERTY(QString, nextUrl)
    QM_PROPERTY(int, commentAccessControl)

public:
    Comments(QObject *parent = nullptr)
        : QObject(parent) { };
    Comments(QObject *parent, QJsonObject data)
        : QObject(parent)
    {
        for (QJsonValue com : data["comments"].toArray()) {
            m_comments.append(new Comment(nullptr, com.toObject()));
        }
        m_nextUrl = data["next"].toString();
        if (data.keys().contains("comment_access_control")) m_commentAccessControl = data["comment_access_control"].toInt();
        else m_commentAccessControl = 0;
    }
};
