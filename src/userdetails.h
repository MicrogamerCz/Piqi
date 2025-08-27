#pragma once
#include "profile.h"
#include "user.h"
#include "workspace.h"

class PIQI_EXPORT UserDetails : public QObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(User*, user)
    QM_PROPERTY(Profile*, profile)
    // profile_publicity
    QM_PROPERTY(Workspace*, workspace)

    public:
        UserDetails(QObject* parent = nullptr);
        UserDetails(QObject* parent, QJsonObject data, QString accessToken = "", QString refreshToken = "");
};
