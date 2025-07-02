#include "privacypolicy.h"

PrivacyPolicy::PrivacyPolicy(QObject* parent) : QObject(parent) {};
PrivacyPolicy::PrivacyPolicy(QObject* parent, QJsonObject data) : QObject(parent)
{
    m_version = data["version"].toString();
    m_message = data["message"].toString();
};
