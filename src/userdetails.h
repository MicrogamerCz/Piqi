#pragma once
#include "profile.h"
#include "user.h"
#include "workspace.h"
#include <qobject.h>

class PIQI_EXPORT UserDetails : public QJObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(User *, user)
    QM_PROPERTY(Profile *, profile)
    // profile_publicity
    QM_PROPERTY(Workspace *, workspace)

  public:
    UserDetails(QObject *parent = nullptr);
    UserDetails(QObject *parent, QJsonObject data);

  private:
    const QStringList properties = {"user", "profile", "workspace"};

    void assignProperty(const QString &propertyname, const QJsonValue &data) override;
};
