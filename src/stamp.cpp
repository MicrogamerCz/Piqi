#include "stamp.h"

Stamp::Stamp(QObject* parent) : QObject(parent) {}
Stamp::Stamp(QObject* parent, QJsonObject data) : QObject(parent)
{
    m_id = data["stamp_id"].toInt();
    m_url = data["stamp_url"].toString();
}
