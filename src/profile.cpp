#include "profile.h"
#include <qjsonobject.h>
#include <qobject.h>
#include <qtmetamacros.h>

Profile::Profile(QObject *parent) : QJObject(parent) {
}
Profile::Profile(QObject *parent, QJsonObject data) : QJObject(data, parent) {
}
