#pragma once
#include "piqi_export.h"
#include "stamp.h"
#include "user.h"

class PIQI_EXPORT Comment : public QObject
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
    Comment(QObject *parent = nullptr);
    Comment(QObject *parent, QJsonObject data);
};
