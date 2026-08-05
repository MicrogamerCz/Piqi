#include "userdetails.h"

UserDetails::UserDetails(QObject *parent) : QJObject(parent) {
}
UserDetails::UserDetails(QObject *parent, QJsonObject data) : QJObject(parent) {
    deserialize(data);
}
void UserDetails::assignProperty(const QString &propertyName, const QJsonValue &data) {
    // TODO: add profile_publicity
    switch (properties.indexOf(propertyName)) {
    case 0: // user
        m_user = new User(this, data.toObject());
        Q_EMIT userChanged();
        break;
    case 1: // profile
        m_profile = new Profile(this, data.toObject());
        Q_EMIT profileChanged();
        break;
    case 2:
        m_workspace = new Workspace(this, data.toObject());
        Q_EMIT workspaceChanged();
        break;
    default:
        QJObject::assignProperty(propertyName, data);
    }
}
