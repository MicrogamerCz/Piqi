#include "userdetails.h"
#include "user.h"
#include "workspace.h"

UserDetails::UserDetails(QObject* parent) : QObject(parent) {}
UserDetails::UserDetails(QObject* parent, QJsonObject data) : QObject(parent) {
    m_user = new User(nullptr, data["user"].toObject());
    m_profile = new Profile(nullptr, data["profile"].toObject());
    // profile_publicity
    m_workspace = new Workspace(nullptr, data["workspace"].toObject());
}
