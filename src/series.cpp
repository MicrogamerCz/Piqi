#include "series.h"

Series::Series(QObject* parent)
    : QObject(parent)
{
}

Series::Series(QObject* parent, QJsonObject data)
    : QObject(parent)
{
    m_id = data["id"].toInt();
    m_title = data["title"].toString();
}
