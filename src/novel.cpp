#include "novel.h"

Novel::Novel(QObject* parent) : Work(parent) { }
Novel::Novel(QObject *parent, QJsonObject data) : Work(parent) {
    deserialize(data);
}

const QString Novel::type() const {
    return "novel";
}
