#include "workspace.h"

Workspace::Workspace(QObject *parent) : QJObject(parent) {
}
Workspace::Workspace(QObject *parent, QJsonObject data) : QJObject(data, parent) {
}
