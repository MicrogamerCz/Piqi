#pragma once
#include "comment.h"
#include "piqi_export.h"
#include "qepr.h"

class PIQI_EXPORT Comments : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QList<Comment *>, comments)
    QM_PROPERTY(QString, nextUrl)
    QM_PROPERTY(int, commentAccessControl)

public:
    Comments(QObject *parent = nullptr);
    Comments(QObject *parent, QJsonObject data);
};
