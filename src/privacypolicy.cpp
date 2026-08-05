#include "privacypolicy.h"

PrivacyPolicy::PrivacyPolicy(QObject *parent) : QJObject(parent) {};
PrivacyPolicy::PrivacyPolicy(QObject *parent, QJsonObject data) : QJObject(data, parent) {};
