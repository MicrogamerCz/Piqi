#include "stamp.h"

Stamp::Stamp(QObject *parent) : QJObject(parent) {
}
Stamp::Stamp(QObject *parent, QJsonObject data) : QJObject(parent) {
    deserialize(data);
}
