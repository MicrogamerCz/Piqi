#include "userdetails.h"
#include "qjobject.h"
#include "user.h"
#include "workspace.h"
#include <qtmetamacros.h>

UserDetails::UserDetails(QObject *parent) : QJObject(parent) {
}
UserDetails::UserDetails(QObject *parent, QJsonObject data) : QJObject(data, parent) {
}

void UserDetails::assignProperty(const QString &propertyName, const QJsonValue &data) {
    // TODO: add profile_publicity
    switch (properties.indexOf(propertyName)) {
    case 0: // user
        m_user = new User(nullptr, data.toObject());
        Q_EMIT userChanged();
        break;
    case 1: // profile
        m_profile = new Profile(nullptr, data.toObject());
        Q_EMIT profileChanged();
        break;
    case 2:
        m_workspace = new Workspace(nullptr, data.toObject());
        Q_EMIT workspaceChanged();
        break;
    default:
        QJObject::assignProperty(propertyName, data);
    }
}
